//
// Created by galex on 5/18/2021.
//

#include "RenderSystem.hpp"
#include "engine/color/HallowColor.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <stdexcept>
#include <array>
#include <application/game_objects/SimpleTriangle.hpp>

namespace Hallow {
  struct PushConstantData {
    alignas(16) glm::mat4 transform{1.0f};
    // alignas(16) glm::vec3 color;
  };

  RenderSystem::RenderSystem(Time& time, HallowDevice& device, VkRenderPass render_pass)
    : m_time{time}, m_hallow_device{device}, m_render_pass{render_pass} {
  }

  RenderSystem::~RenderSystem() {
    vkDestroyPipelineLayout(m_hallow_device.device(), m_pipeline_layout, nullptr);
  }

  void RenderSystem::initilizePipeline() {
    createPipelineLayout();
    createPipeline(m_render_pass);
  }

  void RenderSystem::renderGameObjects(VkCommandBuffer command_buffer,
                                       std::vector<std::shared_ptr<GameObject>>& game_objects) {
    if (!m_pipeline_created) {
      throw std::runtime_error("RenderSystem: Cannot call renderGameObjects before pipeline creation!");
    }

    m_hallow_pipeline->bind(command_buffer);

    for (auto& object : game_objects) {
      // Do stuff to objects
      object->transform().rotation.y = glm::mod(
        object->transform().rotation.y
        + 2.f * static_cast<float>(m_time.delta().seconds()),
        glm::two_pi<float>());
      object->transform().rotation.x = glm::mod(
        object->transform().rotation.x
        + 1.f * static_cast<float>(m_time.delta().seconds()),
        glm::two_pi<float>());

      // Create and fill push constant data
      PushConstantData push{};
      /*push.color = object.color();*/
      push.transform = object->transform().mat4();
      vkCmdPushConstants(
        command_buffer,
        m_pipeline_layout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        0,
        sizeof(PushConstantData),
        &push);

      // Draw object
      object->model()->bind(command_buffer);
      object->model()->draw(command_buffer);
    }
  }

  void RenderSystem::createPipelineLayout() {
    VkPushConstantRange push_constant_range{};
    push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    push_constant_range.offset = 0;
    push_constant_range.size = sizeof(PushConstantData);

    VkPipelineLayoutCreateInfo pipeline_layout_info{};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 0;
    pipeline_layout_info.pSetLayouts = nullptr;
    pipeline_layout_info.pushConstantRangeCount = 1;
    pipeline_layout_info.pPushConstantRanges = &push_constant_range;

    if (vkCreatePipelineLayout(m_hallow_device.device(), &pipeline_layout_info, nullptr, &m_pipeline_layout) !=
        VK_SUCCESS) {
      throw std::runtime_error("HallowApp: Failed to create pipeline layout!");
    }
  }

  void RenderSystem::createPipeline(VkRenderPass render_pass) {
    if (m_pipeline_layout == nullptr) {
      throw std::runtime_error("HallowApp: Attempted to create pipeline while pipeline layout is nullptr!");
    }

    PipelineConfigInfo pipeline_config{};
    HallowPipeline::defaultPipelineConfig(pipeline_config);

    // render pass describes the structure and format of frame buffer objects and their attachments
    pipeline_config.render_pass = render_pass;
    pipeline_config.pipeline_layout = m_pipeline_layout;

    m_hallow_pipeline = std::make_unique<HallowPipeline>(m_hallow_device, pipeline_config, m_shader_path);

    m_pipeline_created = true;
  }
}