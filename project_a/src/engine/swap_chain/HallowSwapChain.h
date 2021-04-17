#pragma once

#include "engine/device/HallowDevice.h"

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <string>
#include <vector>


namespace Hallow {

  class HallowSwapChain {
  public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    HallowSwapChain(HallowDevice& device_ref, VkExtent2D windowExtent);
    ~HallowSwapChain();

    VkFramebuffer frameBuffer(int index) { return m_swap_chain_framebuffers[index]; }

    VkRenderPass renderPass() { return m_render_pass; }

    VkImageView imageView(int index) { return m_swap_chain_image_views[index]; }

    size_t imageCount() { return m_swap_chain_images.size(); }

    VkFormat swapChainImageFormat() { return m_swap_chain_image_format; }

    VkExtent2D swapChainExtent() { return m_swap_chain_extent; }

    uint32_t width() const { return m_swap_chain_extent.width; }

    uint32_t height() const { return m_swap_chain_extent.height; }

    float extentAspectRatio() {
      return static_cast<float>(m_swap_chain_extent.width) /
             static_cast<float>(m_swap_chain_extent.height);
    }

    VkFormat findDepthFormat();
    VkResult acquireNextImage(uint32_t* image_index);
    VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* image_index);

    HallowSwapChain(const HallowSwapChain&) = delete;
    void operator=(const HallowSwapChain&) = delete;
  private:
    HallowDevice& m_hallow_device;
    VkSwapchainKHR m_swap_chain;
    VkExtent2D m_window_extent;
    VkPresentModeKHR m_preferred_present_mode{VK_PRESENT_MODE_IMMEDIATE_KHR};
    VkFormat m_swap_chain_image_format;
    VkExtent2D m_swap_chain_extent;
    std::vector<VkFramebuffer> m_swap_chain_framebuffers;
    VkRenderPass m_render_pass;
    std::vector<VkImage> m_depth_images;
    std::vector<VkDeviceMemory> m_depth_image_memorys;
    std::vector<VkImageView> m_depth_image_views;
    std::vector<VkImage> m_swap_chain_images;
    std::vector<VkImageView> m_swap_chain_image_views;
    std::vector<VkSemaphore> m_image_available_semaphores;
    std::vector<VkSemaphore> m_render_finished_semaphores;
    std::vector<VkFence> m_in_flight_fences;
    std::vector<VkFence> m_images_in_flight;
    size_t m_current_frame = 0;

    void createSwapChain();
    void createImageViews();
    void createDepthResources();
    void createRenderPass();
    void createFramebuffers();
    void createSyncObjects();

    // Helper functions
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& available_formats);

    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& available_present_modes,
        VkPresentModeKHR preferred_mode);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
  };

}  // namespace Hallow
