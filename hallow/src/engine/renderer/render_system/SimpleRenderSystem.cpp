//
// Created by galex on 5/18/2021.
//

#include "SimpleRenderSystem.hpp"
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
  struct SimplePushConstantData {
    alignas(16) glm::mat2 transform{1.0f};
    alignas(8) glm::vec2 offset;
    alignas(16) glm::vec3 color;
  };

  SimpleRenderSystem::SimpleRenderSystem(Time& time, HallowDevice& device, VkRenderPass render_pass, RendererOptions renderer_options)
    : m_time{time}, m_hallow_device{device}, m_renderer_options{renderer_options} {
    createPipelineLayout();
    createPipeline(render_pass);
  }

  SimpleRenderSystem::~SimpleRenderSystem() {
    vkDestroyPipelineLayout(m_hallow_device.device(), m_pipeline_layout, nullptr);
  }

  void SimpleRenderSystem::renderGameObjects(VkCommandBuffer command_buffer,
                                             std::vector<HallowGameObject>& game_objects) {
    m_hallow_pipeline->bind(command_buffer);

    for (auto& object : game_objects) {
      /*object.transform().translation = {.2f, .0f};
      object.transform().scale = {2.f, .5f};*/
      object.transform().rotation = glm::mod(
        object.transform().rotation + 2.f * static_cast<float>(m_time.delta().seconds()),
        glm::two_pi<float>());

      SimplePushConstantData push{};
      push.offset = object.transform().translation;
      push.color = object.color();
      push.transform = object.transform().mat2();

      vkCmdPushConstants(
        command_buffer,
        m_pipeline_layout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        0,
        sizeof(SimplePushConstantData),
        &push);

      object.model()->bind(command_buffer);
      object.model()->draw(command_buffer);
    }
  }

  void SimpleRenderSystem::createPipelineLayout() {
    VkPushConstantRange push_constant_range{};
    push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    push_constant_range.offset = 0;
    push_constant_range.size = sizeof(SimplePushConstantData);

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

  void SimpleRenderSystem::createPipeline(VkRenderPass render_pass) {
    if (m_pipeline_layout == nullptr) {
      throw std::runtime_error("HallowApp: Attempted to create pipeline while pipeline layout is nullptr!");
    }

    PipelineConfigInfo pipeline_config{};
    HallowPipeline::defaultPipelineConfig(pipeline_config);

    // render pass describes the structure and format of frame buffer objects and their attachments
    pipeline_config.render_pass = render_pass;
    pipeline_config.pipeline_layout = m_pipeline_layout;

    m_hallow_pipeline = std::make_unique<HallowPipeline>(m_hallow_device, pipeline_config, "res/shaders/simple_shader");
  }
}