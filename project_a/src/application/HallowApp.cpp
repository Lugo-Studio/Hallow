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
    vkDestroyPipelineLayout(m_hallow_device.device(), m_pipeline_layout,
                            nullptr);

  }

  void HallowApp::run() {
    startPre();
    start();
    startPost();

    while (!m_hallow_window.shouldClose()) {
      m_time.tick(); // start with tick to get newest time
      //m_time.printOnInterval(1);

      glfwPollEvents(); // poll for user events and such

      updatePre();
      update();
      updatePost();

      drawFrame();

      // TODO: late update?
    }

    endPre();
    end();
    endPost();

    vkDeviceWaitIdle(m_hallow_device.device());
  }

  void HallowApp::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(m_hallow_device.device(),
                               &pipelineLayoutInfo, nullptr,
                               &m_pipeline_layout) != VK_SUCCESS) {
      throw std::runtime_error(
              "HallowApp: Failed to create pipeline layout!");
    }
  }

  void HallowApp::createPipeline() {
    PipelineConfigInfo pipelineConfig{};

    HallowPipeline::defaultPipelineConfig(
            pipelineConfig,
            m_hallow_swap_chain.width(),
            m_hallow_swap_chain.height());

    // render pass describes the structure and format of frame buffer objects and their attachments
    pipelineConfig.renderPass = m_hallow_swap_chain.getRenderPass();
    pipelineConfig.pipelineLayout = m_pipeline_layout;

    m_hallow_pipeline = std::make_unique<HallowPipeline>(m_hallow_device,
                                                         pipelineConfig,
                                                         "res/shaders/simple_shader");
  }

  void HallowApp::createCommandBuffers() {
    m_command_buffers.resize(m_hallow_swap_chain.imageCount());

    VkCommandBufferAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandPool = m_hallow_device.getCommandPool();
    allocateInfo.commandBufferCount = static_cast<uint32_t>(m_command_buffers.size());

    if (vkAllocateCommandBuffers(m_hallow_device.device(), &allocateInfo,
                                 m_command_buffers.data()) != VK_SUCCESS) {
      throw std::runtime_error(
              "HallowApp: Failed to allocate command buffers!");
    }

    for (int i = 0; i < m_command_buffers.size(); ++i) {
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      if (vkBeginCommandBuffer(m_command_buffers[i], &beginInfo) !=
          VK_SUCCESS) {
        throw std::runtime_error(
                "HallowApp: Failed to begin recording command buffer!");
      }

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = m_hallow_swap_chain.getRenderPass();
      renderPassInfo.framebuffer = m_hallow_swap_chain.getFrameBuffer(i);

      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = m_hallow_swap_chain.getSwapChainExtent();

      Color backgroundColor{0x2E3440FF};
      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color = {backgroundColor.r, backgroundColor.g,
                              backgroundColor.b, backgroundColor.a};
      clearValues[1].depthStencil = {1.0f, 0};

      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues = clearValues.data();

      vkCmdBeginRenderPass(m_command_buffers[i], &renderPassInfo,
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
    uint32_t imageIndex;
    auto result = m_hallow_swap_chain.acquireNextImage(&imageIndex);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error(
              "HallowApp: Failed to acquire next swap chain image!");
    }

    result = m_hallow_swap_chain.submitCommandBuffers(
            &m_command_buffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS) {
      throw std::runtime_error(
              "HallowApp: Failed to present swap chain image!");
    }
  }

  // lifetime class?
  void HallowApp::startPre() {}

  void HallowApp::start() {}

  void HallowApp::startPost() {}

  void HallowApp::updatePre() {}

  void HallowApp::update() {}

  void HallowApp::updatePost() {}

  void HallowApp::endPre() {}

  void HallowApp::end() {}

  void HallowApp::endPost() {}
}