#pragma once

#include "engine/device/HallowDevice.hpp"

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <memory>
#include <string>
#include <vector>
#include <engine/renderer/RendererOptions.hpp>


namespace Hallow {

  class HallowSwapChain {
  public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    HallowSwapChain(HallowDevice& device_ref, VkExtent2D window_extent, RendererOptions renderer_options);
    HallowSwapChain(HallowDevice& device_ref,
                    VkExtent2D window_extent,
                    RendererOptions renderer_options,
                    std::shared_ptr<HallowSwapChain> previous);
    ~HallowSwapChain();
    HallowSwapChain(const HallowSwapChain&) = delete;
    HallowSwapChain& operator=(const HallowSwapChain&) = delete;

    VkFramebuffer frameBuffer(int index) { return m_swap_chain_framebuffers[index]; }
    VkRenderPass renderPass() { return m_render_pass; }
    VkImageView imageView(int index) { return m_swap_chain_image_views[index]; }
    size_t imageCount() { return m_swap_chain_images.size(); }
    VkFormat swapChainImageFormat() { return m_swap_chain_image_format; }
    VkExtent2D swapChainExtent() { return m_swap_chain_extent; }
    uint32_t width() const { return m_swap_chain_extent.width; }
    uint32_t height() const { return m_swap_chain_extent.height; }
    float extentAspectRatio() const {
      return static_cast<float>(m_swap_chain_extent.width) / static_cast<float>(m_swap_chain_extent.height);
    }
    //bool usingSrgbColorSpace() const { return m_using_srgb_color_space; }
    VkFormat findDepthFormat();
    VkResult acquireNextImage(uint32_t* image_index);
    VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* image_index);

    bool compareSwapChainFormats(const HallowSwapChain& swap_chain) const {
      return swap_chain.m_swap_chain_image_format == m_swap_chain_image_format
             && swap_chain.m_swap_cahin_depth_format == m_swap_cahin_depth_format;
    }
  private:
    std::shared_ptr<HallowSwapChain> m_old_swap_chain;
    HallowDevice& m_hallow_device;
    VkSwapchainKHR m_swap_chain;
    VkExtent2D m_window_extent;
    VkFormat m_swap_chain_image_format;
    VkFormat m_swap_cahin_depth_format;
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

    // VkPresentModeKHR m_preferred_present_mode{VK_PRESENT_MODE_IMMEDIATE_KHR};
    RendererOptions m_renderer_options;
    // VkSurfaceFormatKHR m_preferred_surface_format{VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};

    void init();
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
      const std::vector<VkPresentModeKHR>& available_present_modes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
  };

}  // namespace Hallow
