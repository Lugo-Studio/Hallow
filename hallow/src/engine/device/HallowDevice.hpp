#pragma once

#include <engine/window/HallowWindow.hpp>

// std lib headers
#include <string>
#include <vector>


namespace Hallow {

  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
  };

  struct QueueFamilyIndices {
    uint32_t graphics_family;
    uint32_t present_family;
    bool graphics_family_has_value = false;
    bool present_family_has_value = false;

    bool isComplete() {
      return graphics_family_has_value && present_family_has_value;
    }
  };

  class HallowDevice {
  public:
    #ifdef NDEBUG
    const bool ENABLE_VALIDATION_LAYERS = false;
    #else
    const bool ENABLE_VALIDATION_LAYERS = true;
    #endif

    VkPhysicalDeviceProperties properties;

    HallowDevice(HallowWindow& window);
    ~HallowDevice();

    VkCommandPool commandPool() { return m_command_pool; }

    VkDevice device() { return m_device; }

    VkSurfaceKHR surface() { return m_surface; }

    VkQueue graphicsQueue() { return m_graphics_queue; }

    VkQueue presentQueue() { return m_present_queue; }

    SwapChainSupportDetails swapChainSupport() {
      return querySwapChainSupport(m_physical_device);
    }

    uint32_t findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties);

    QueueFamilyIndices findPhysicalQueueFamilies() {
      return findQueueFamilies(m_physical_device);
    }

    VkFormat findSupportedFormat(
      const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    // Buffer Helper Functions
    void createBuffer(
      VkDeviceSize size,
      VkBufferUsageFlags usage,
      VkMemoryPropertyFlags mem_properties,
      VkBuffer& buffer,
      VkDeviceMemory& buffer_memory);
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer command_buffer);
    void copyBuffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size);
    void copyBufferToImage(
      VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layer_count);
    void createImageWithInfo(
      const VkImageCreateInfo& image_info,
      VkMemoryPropertyFlags mem_properties,
      VkImage& image,
      VkDeviceMemory& image_memory);

    // Not copyable or movable
    HallowDevice(const HallowDevice&) = delete;
    HallowDevice& operator=(const HallowDevice&) = delete;
    HallowDevice(HallowDevice&&) = delete;
    HallowDevice& operator=(HallowDevice&&) = delete;
  private:
    void createInstance();

    void setupDebugMessenger();

    void createSurface();

    void pickPhysicalDevice();

    void createLogicalDevice();

    void createCommandPool();

    // helper functions
    bool isDeviceSuitable(VkPhysicalDevice device);
    std::vector<const char*> requiredExtensions();
    bool checkValidationLayerSupport();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);
    void hasGflwRequiredInstanceExtensions();
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    HallowWindow& m_hallow_window;
    VkDevice m_device;
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debug_messenger;
    VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
    VkCommandPool m_command_pool;
    VkSurfaceKHR m_surface;
    VkQueue m_graphics_queue;
    VkQueue m_present_queue;

    const std::vector<const char*> VALIDATION_LAYERS = {
      "VK_LAYER_KHRONOS_validation"
    };
    const std::vector<const char*> DEVICE_EXTENSIONS = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
  };

}  // namespace Hallow