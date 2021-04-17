//
// Created by Gabriel Lugo on 3/28/2021.
//

#include "HallowApp.h"
#include "engine/color/HallowColor.h"

#include <iostream>
#include <stdexcept>
#include <array>


namespace Hallow {

  HallowApp::HallowApp() {
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
  }

  HallowApp::~HallowApp() {
    vkDestroyPipelineLayout(m_hallow_device.device(),
                            m_pipeline_layout,
                            nullptr);

  }

  void HallowApp::run() {
    onStart();

    while (!m_hallow_window.shouldClose()) {
      onUpdate();
    }

    onEnd();
  }

  void HallowApp::onStart() {}

  void HallowApp::onUpdate() {
    m_time.tick(); // onStart with tick to get newest time
    //m_time.printOnInterval(1);

    glfwPollEvents(); // poll for user events and such


    drawFrame();
  }

  void HallowApp::onEnd() {
    vkDeviceWaitIdle(m_hallow_device.device());
  }

  void HallowApp::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipeline_layout_info{};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 0;
    pipeline_layout_info.pSetLayouts = nullptr;
    pipeline_layout_info.pushConstantRangeCount = 0;
    pipeline_layout_info.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(m_hallow_device.device(),
                               &pipeline_layout_info, nullptr,
                               &m_pipeline_layout) != VK_SUCCESS) {
      throw std::runtime_error(
          "HallowApp: Failed to create pipeline layout!");
    }
  }

  void HallowApp::createPipeline() {
    PipelineConfigInfo pipeline_config{};

    HallowPipeline::defaultPipelineConfig(
        pipeline_config,
        m_hallow_swap_chain.width(),
        m_hallow_swap_chain.height());

    // render pass describes the structure and format of frame buffer objects and their attachments
    pipeline_config.render_pass = m_hallow_swap_chain.renderPass();
    pipeline_config.pipeline_layout = m_pipeline_layout;

    m_hallow_pipeline = std::make_unique<HallowPipeline>(m_hallow_device,
                                                         pipeline_config,
                                                         "res/shaders/simple_shader");
  }

  void HallowApp::createCommandBuffers() {
    m_command_buffers.resize(m_hallow_swap_chain.imageCount());

    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = m_hallow_device.commandPool();
    alloc_info.commandBufferCount = static_cast<uint32_t>(m_command_buffers.size());

    if (vkAllocateCommandBuffers(m_hallow_device.device(), &alloc_info,
                                 m_command_buffers.data()) != VK_SUCCESS) {
      throw std::runtime_error(
          "HallowApp: Failed to allocate command buffers!");
    }

    for (int i = 0; i < m_command_buffers.size(); ++i) {
      VkCommandBufferBeginInfo begin_info{};
      begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      if (vkBeginCommandBuffer(m_command_buffers[i], &begin_info) !=
          VK_SUCCESS) {
        throw std::runtime_error(
            "HallowApp: Failed to begin recording command buffer!");
      }

      VkRenderPassBeginInfo render_pass_info{};
      render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      render_pass_info.renderPass = m_hallow_swap_chain.renderPass();
      render_pass_info.framebuffer = m_hallow_swap_chain.frameBuffer(i);

      render_pass_info.renderArea.offset = {0, 0};
      render_pass_info.renderArea.extent = m_hallow_swap_chain.swapChainExtent();

      Color background_color{0x2E3440FF};
      std::array<VkClearValue, 2> clear_values{};
      clear_values[0].color = {background_color.r, background_color.g,
                               background_color.b, background_color.a};
      clear_values[1].depthStencil = {1.0f, 0};

      render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
      render_pass_info.pClearValues = clear_values.data();

      vkCmdBeginRenderPass(m_command_buffers[i], &render_pass_info,
                           VK_SUBPASS_CONTENTS_INLINE);

      m_hallow_pipeline->bind(m_command_buffers[i]);
      vkCmdDraw(m_command_buffers[i],
                3,
                1,
                0,
                0);

      vkCmdEndRenderPass(m_command_buffers[i]);
      if (vkEndCommandBuffer(m_command_buffers[i]) != VK_SUCCESS) {
        throw std::runtime_error(
            "HallowApp: Failed to record command buffer!");
      }
    }
  }

  void HallowApp::drawFrame() {
    uint32_t image_index;
    auto result = m_hallow_swap_chain.acquireNextImage(&image_index);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error(
          "HallowApp: Failed to acquire next swap chain image!");
    }

    result = m_hallow_swap_chain.submitCommandBuffers(
        &m_command_buffers[image_index], &image_index);
    if (result != VK_SUCCESS) {
      throw std::runtime_error(
          "HallowApp: Failed to present swap chain image!");
    }
  }
}
