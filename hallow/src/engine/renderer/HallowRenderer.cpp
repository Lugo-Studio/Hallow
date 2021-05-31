//
// Created by galex on 5/18/2021.
//

#include <engine/color/HallowColor.hpp>
#include <array>
#include "HallowRenderer.hpp"

namespace Hallow {

  HallowRenderer::HallowRenderer(HallowWindow& window, HallowDevice& device)
    : m_hallow_window{window}, m_hallow_device{device} {
    recreateSwapChain();
    createCommandBuffers();
  }

  HallowRenderer::~HallowRenderer() {
    freeCommandBuffers();
  }

  VkCommandBuffer HallowRenderer::beginFrame() {
    if (isFrameInProgress()) {
      throw std::runtime_error("HallowRenderer: Cannot call beginFrame while frame is already in progress!");
    }

    auto result = m_hallow_swap_chain->acquireNextImage(&m_current_image_index);

    // when something has changed to cause swapchain to no longer be valid (such as window resizing)
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      recreateSwapChain();
      return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("HallowApp: Failed to acquire next swap chain image!");
    }

    m_is_frame_started = true;

    auto command_buffer = currentCommandBuffer();
    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
      throw std::runtime_error("HallowApp: Failed to begin recording command buffer!");
    }

    return command_buffer;
  }

  void HallowRenderer::endFrame() {
    if (!isFrameInProgress()) {
      throw std::runtime_error("HallowRenderer: Cannot call endFrame while frame is not in progress!");
    }

    auto command_buffer = currentCommandBuffer();

    if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
      throw std::runtime_error("HallowApp: Failed to record command buffer!");
    }

    auto result = m_hallow_swap_chain->submitCommandBuffers(&command_buffer, &m_current_image_index);

    // when something has changed to cause swapchain to no longer be valid or is no longer valid but can still
    // present to the surface (such as window resizing)
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_hallow_window.wasWindowResized()) {
      m_hallow_window.resetResizedFlag();
      recreateSwapChain();
    } else if (result != VK_SUCCESS) {
      std::cerr << result << "\n";
      throw std::runtime_error("HallowApp: Failed to present swap chain image!");
    }

    m_is_frame_started = false;
    m_current_frame_index = (m_current_frame_index + 1) % HallowSwapChain::MAX_FRAMES_IN_FLIGHT;
  }

  void HallowRenderer::beginSwapChainRenderPass(VkCommandBuffer command_buffer) {
    if (!isFrameInProgress()) {
      throw std::runtime_error("HallowRenderer: Cannot call beginSwapChainRenderPass while frame is not in progress!");
    }
    if (command_buffer != currentCommandBuffer()) {
      throw std::runtime_error("HallowRenderer: Cannot begin render pass on command buffer from a different frame!");
    }

    VkRenderPassBeginInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = m_hallow_swap_chain->renderPass();
    render_pass_info.framebuffer = m_hallow_swap_chain->frameBuffer(m_current_image_index);

    render_pass_info.renderArea.offset = {0, 0};
    render_pass_info.renderArea.extent = m_hallow_swap_chain->swapChainExtent();

    HallowColor background_color(0x2E3440FF);
    std::array<VkClearValue, 2> clear_values{};
    clear_values[0].color = background_color.color_rgba<VkClearColorValue>();
    clear_values[1].depthStencil = {1.0f, 0};

    render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
    render_pass_info.pClearValues = clear_values.data();

    vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_hallow_swap_chain->swapChainExtent().width);
    viewport.height = static_cast<float>(m_hallow_swap_chain->swapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0},
                     m_hallow_swap_chain->swapChainExtent()};
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);
  }

  void HallowRenderer::endSwapChainRenderPass(VkCommandBuffer command_buffer) {
    if (!isFrameInProgress()) {
      throw std::runtime_error("HallowRenderer: Cannot call endSwapChainRenderPass while frame is not in progress!");
    }
    if (command_buffer != currentCommandBuffer()) {
      throw std::runtime_error("HallowRenderer: Cannot end render pass on command buffer from a different frame!");
    }

    vkCmdEndRenderPass(command_buffer);
  }

  void HallowRenderer::createCommandBuffers() {
    m_command_buffers.resize(HallowSwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = m_hallow_device.commandPool();
    alloc_info.commandBufferCount = static_cast<uint32_t>(m_command_buffers.size());

    if (vkAllocateCommandBuffers(m_hallow_device.device(), &alloc_info, m_command_buffers.data()) != VK_SUCCESS) {
      throw std::runtime_error("HallowApp: Failed to allocate command buffers!");
    }
  }

  void HallowRenderer::freeCommandBuffers() {
    vkFreeCommandBuffers(
      m_hallow_device.device(),
      m_hallow_device.commandPool(),
      static_cast<uint32_t>(m_command_buffers.size()),
      m_command_buffers.data()
    );
    m_command_buffers.clear();
  }

  void HallowRenderer::recreateSwapChain() {
    auto extent = m_hallow_window.extent();

    // pause and wait while at least one dimension is sizeless (such as when minimized)
    while (extent.width == 0 || extent.height == 0) {
      extent = m_hallow_window.extent();
      glfwWaitEvents();
    }

    // wait until current swap chain is no longer being used
    vkDeviceWaitIdle(m_hallow_device.device());

    if (m_hallow_swap_chain == nullptr) {
      m_hallow_swap_chain = std::make_unique<HallowSwapChain>(m_hallow_device, extent);
    } else {
      std::shared_ptr<HallowSwapChain> old_swap_chain = std::move(m_hallow_swap_chain);
      m_hallow_swap_chain = std::make_unique<HallowSwapChain>(
        m_hallow_device,
        extent,
        old_swap_chain
      );

      if (!old_swap_chain->compareSwapChainFormats(*m_hallow_swap_chain)) {
        throw std::runtime_error("HallowRenderer: Swap chain image or depth format has changed!");
      }
    }

    // TODO: keep pipeline if it is compatible: https://youtu.be/0IIqvi3Z0ng?t=744
    // createPipeline();
  }
}