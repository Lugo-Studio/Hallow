//
// Created by Gabriel Lugo on 3/28/2021.
//

#include "HallowApp.hpp"
#include "engine/color/HallowColor.hpp"

#include <iostream>
#include <stdexcept>
#include <array>


namespace Hallow {

  HallowApp::HallowApp() {
    loadModels();
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
  }

  HallowApp::~HallowApp() {
    vkDestroyPipelineLayout(m_hallow_device.device(), m_pipeline_layout, nullptr);
  }

  void HallowApp::run() {
    onStart();

    while (!m_hallow_window.shouldClose()) {
      onUpdate();
    }

    onEnd();
  }

  void HallowApp::onStart() {
    // Pre-start
    m_hallow_game.onPreStart();

    // Start
    m_hallow_game.onStart();

    // Post-start
    m_hallow_game.onPostStart();
  }

  void HallowApp::onUpdate() {
    // Pre-update
    m_hallow_game.onPreUpdate();

    // Update
    m_time.tick(); // onStart with tick to get newest time
    //m_time.printOnInterval(1);
    glfwPollEvents(); // poll for user events and such
    m_hallow_game.onUpdate();

    // Post-update
    drawFrame();
    m_hallow_game.onPostUpdate();
  }

  void HallowApp::onEnd() {
    // Pre-end
    m_hallow_game.onPreEnd();

    // End
    vkDeviceWaitIdle(m_hallow_device.device());
    m_hallow_game.onEnd();

    // Post-end
    m_hallow_game.onPostEnd();
  }

  HallowModel::Vertex midpoint(
      const glm::vec2& v1,
      const glm::vec2& v2) {
    return {{(v1[0] + v2[0]) * 0.5f, (v1[1] + v2[1]) * 0.5f}};
  }

  void sierpinski(
      std::vector<HallowModel::Vertex>& vertices,
      const HallowModel::Triangle& triangle,
      int depth) {
    HallowModel::Vertex midpoints[] = {
        midpoint(triangle.vertices[0].position, triangle.vertices[1].position),
        midpoint(triangle.vertices[1].position, triangle.vertices[2].position),
        midpoint(triangle.vertices[2].position, triangle.vertices[0].position)
    };

    if (depth == 0) {
      for (const auto& vert : triangle.vertices) {
        vertices.push_back(vert);
      }
    } else {
      --depth;
      HallowModel::Triangle triangles[] = {
          {triangle.vertices[0],
              midpoints[0],
              midpoints[2]},
          {midpoints[0],
              triangle.vertices[1],
              midpoints[1]},
          {midpoints[2],
              midpoints[1],
              triangle.vertices[2]}
      };

      for (const auto& tri : triangles) {
        sierpinski(vertices, tri, depth);
      }
    }
  }

  void HallowApp::loadModels() {
    HallowModel::Triangle triangle{};
    triangle.vertices[0].position = {-0.5, 0.5};
    triangle.vertices[1].position = {0.5, 0.5};
    triangle.vertices[2].position = {0.0, -0.5};

    std::vector<HallowModel::Vertex> vertices{};
    sierpinski(vertices, triangle, 7);

    m_hallow_model = std::make_unique<HallowModel>(m_hallow_device, vertices);
  }

  void HallowApp::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipeline_layout_info{};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 0;
    pipeline_layout_info.pSetLayouts = nullptr;
    pipeline_layout_info.pushConstantRangeCount = 0;
    pipeline_layout_info.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(m_hallow_device.device(), &pipeline_layout_info, nullptr, &m_pipeline_layout) !=
        VK_SUCCESS) {
      throw std::runtime_error("HallowApp: Failed to create pipeline layout!");
    }
  }

  void HallowApp::createPipeline() {
    PipelineConfigInfo pipeline_config{};

    HallowPipeline::defaultPipelineConfig(pipeline_config, m_hallow_swap_chain.width(), m_hallow_swap_chain.height());

    // render pass describes the structure and format of frame buffer objects and their attachments
    pipeline_config.render_pass = m_hallow_swap_chain.renderPass();
    pipeline_config.pipeline_layout = m_pipeline_layout;

    m_hallow_pipeline = std::make_unique<HallowPipeline>(m_hallow_device, pipeline_config, "res/shaders/simple_shader");
  }

  void HallowApp::createCommandBuffers() {
    m_command_buffers.resize(m_hallow_swap_chain.imageCount());

    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = m_hallow_device.commandPool();
    alloc_info.commandBufferCount = static_cast<uint32_t>(m_command_buffers.size());

    if (vkAllocateCommandBuffers(m_hallow_device.device(), &alloc_info, m_command_buffers.data()) != VK_SUCCESS) {
      throw std::runtime_error("HallowApp: Failed to allocate command buffers!");
    }

    for (int i = 0; i < m_command_buffers.size(); ++i) {
      VkCommandBufferBeginInfo begin_info{};
      begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      if (vkBeginCommandBuffer(m_command_buffers[i], &begin_info) != VK_SUCCESS) {
        throw std::runtime_error("HallowApp: Failed to begin recording command buffer!");
      }

      VkRenderPassBeginInfo render_pass_info{};
      render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      render_pass_info.renderPass = m_hallow_swap_chain.renderPass();
      render_pass_info.framebuffer = m_hallow_swap_chain.frameBuffer(i);

      render_pass_info.renderArea.offset = {0, 0};
      render_pass_info.renderArea.extent = m_hallow_swap_chain.swapChainExtent();

      Color background_color{0x2E3440FF};
      std::array<VkClearValue, 2> clear_values{};
      clear_values[0].color = {
          background_color.r, background_color.g, background_color.b, background_color.a
      };
      clear_values[1].depthStencil = {1.0f, 0};

      render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
      render_pass_info.pClearValues = clear_values.data();

      vkCmdBeginRenderPass(m_command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

      m_hallow_pipeline->bind(m_command_buffers[i]);
      m_hallow_model->bind(m_command_buffers[i]);
      m_hallow_model->draw(m_command_buffers[i]);

      vkCmdEndRenderPass(m_command_buffers[i]);
      if (vkEndCommandBuffer(m_command_buffers[i]) != VK_SUCCESS) {
        throw std::runtime_error("HallowApp: Failed to record command buffer!");
      }
    }
  }

  void HallowApp::drawFrame() {
    uint32_t image_index;
    auto result = m_hallow_swap_chain.acquireNextImage(&image_index);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("HallowApp: Failed to acquire next swap chain image!");
    }

    result = m_hallow_swap_chain.submitCommandBuffers(&m_command_buffers[image_index], &image_index);
    if (result != VK_SUCCESS) {
      throw std::runtime_error("HallowApp: Failed to present swap chain image!");
    }
  }
}
