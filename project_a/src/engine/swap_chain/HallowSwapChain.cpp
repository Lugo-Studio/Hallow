#include "HallowSwapChain.h"

// std
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>


namespace Hallow {

  HallowSwapChain::HallowSwapChain(HallowDevice& deviceRef, VkExtent2D extent)
          : m_hallow_device{deviceRef}, m_window_extent{extent} {
    createSwapChain();
    createImageViews();
    createRenderPass();
    createDepthResources();
    createFramebuffers();
    createSyncObjects();
  }

  HallowSwapChain::~HallowSwapChain() {
    for (auto imageView : m_swap_chain_image_views) {
      vkDestroyImageView(m_hallow_device.device(), imageView, nullptr);
    }
    m_swap_chain_image_views.clear();

    if (m_swap_chain != (VkSwapchainKHR) nullptr) {
      vkDestroySwapchainKHR(m_hallow_device.device(), m_swap_chain, nullptr);
      m_swap_chain = (VkSwapchainKHR) nullptr;
    }

    for (int i = 0; i < m_depth_images.size(); i++) {
      vkDestroyImageView(m_hallow_device.device(), m_depth_image_views[i], nullptr);
      vkDestroyImage(m_hallow_device.device(), m_depth_images[i], nullptr);
      vkFreeMemory(m_hallow_device.device(), m_depth_image_memorys[i], nullptr);
    }

    for (auto framebuffer : m_swap_chain_framebuffers) {
      vkDestroyFramebuffer(m_hallow_device.device(), framebuffer, nullptr);
    }

    vkDestroyRenderPass(m_hallow_device.device(), m_render_pass, nullptr);

    // cleanup synchronization objects
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      vkDestroySemaphore(m_hallow_device.device(), m_render_finished_semaphores[i],
                         nullptr);
      vkDestroySemaphore(m_hallow_device.device(), m_image_available_semaphores[i],
                         nullptr);
      vkDestroyFence(m_hallow_device.device(), m_in_flight_fences[i], nullptr);
    }
  }

  VkResult HallowSwapChain::acquireNextImage(uint32_t* imageIndex) {
    vkWaitForFences(
            m_hallow_device.device(),
            1,
            &m_in_flight_fences[m_current_frame],
            VK_TRUE,
            std::numeric_limits<uint64_t>::max());

    VkResult result = vkAcquireNextImageKHR(
            m_hallow_device.device(),
            m_swap_chain,
            std::numeric_limits<uint64_t>::max(),
            m_image_available_semaphores[m_current_frame],  // must be a not signaled semaphore
            VK_NULL_HANDLE,
            imageIndex);

    return result;
  }

  VkResult HallowSwapChain::submitCommandBuffers(
          const VkCommandBuffer* buffers, uint32_t* imageIndex) {
    if (m_images_in_flight[*imageIndex] != VK_NULL_HANDLE) {
      vkWaitForFences(m_hallow_device.device(), 1, &m_images_in_flight[*imageIndex],
                      VK_TRUE, UINT64_MAX);
    }
    m_images_in_flight[*imageIndex] = m_in_flight_fences[m_current_frame];

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[]{m_image_available_semaphores[m_current_frame]};
    VkPipelineStageFlags waitStages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = buffers;

    VkSemaphore signalSemaphores[]{m_render_finished_semaphores[m_current_frame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(m_hallow_device.device(), 1, &m_in_flight_fences[m_current_frame]);
    if (vkQueueSubmit(m_hallow_device.graphicsQueue(), 1, &submitInfo,
                      m_in_flight_fences[m_current_frame]) !=
        VK_SUCCESS) {
      throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[]{m_swap_chain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = imageIndex;

    auto result = vkQueuePresentKHR(m_hallow_device.presentQueue(), &presentInfo);

    m_current_frame = (m_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;

    return result;
  }

  void HallowSwapChain::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = m_hallow_device.swapChainSupport();

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(
            swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(
            swapChainSupport.present_modes, m_preferred_present_mode);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapChainSupport.capabilities.maxImageCount) {
      imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_hallow_device.surface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = m_hallow_device.findPhysicalQueueFamilies();
    uint32_t queueFamilyIndices[] = {indices.graphics_family,
                                     indices.present_family};

    if (indices.graphics_family != indices.present_family) {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      createInfo.queueFamilyIndexCount = 0;      // Optional
      createInfo.pQueueFamilyIndices = nullptr;  // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_hallow_device.device(), &createInfo, nullptr,
                             &m_swap_chain) != VK_SUCCESS) {
      throw std::runtime_error("failed to create swap chain!");
    }

    // we only specified a minimum number of images in the swap chain, so the implementation is
    // allowed to create a swap chain with more. That's why we'll first query the final number of
    // images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
    // retrieve the handles.
    vkGetSwapchainImagesKHR(m_hallow_device.device(), m_swap_chain, &imageCount,
                            nullptr);
    m_swap_chain_images.resize(imageCount);
    vkGetSwapchainImagesKHR(m_hallow_device.device(), m_swap_chain, &imageCount,
                            m_swap_chain_images.data());

    m_swap_chain_image_format = surfaceFormat.format;
    m_swap_chain_extent = extent;
  }

  void HallowSwapChain::createImageViews() {
    m_swap_chain_image_views.resize(m_swap_chain_images.size());
    for (size_t i = 0; i < m_swap_chain_images.size(); i++) {
      VkImageViewCreateInfo viewInfo{};
      viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      viewInfo.image = m_swap_chain_images[i];
      viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      viewInfo.format = m_swap_chain_image_format;
      viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      viewInfo.subresourceRange.baseMipLevel = 0;
      viewInfo.subresourceRange.levelCount = 1;
      viewInfo.subresourceRange.baseArrayLayer = 0;
      viewInfo.subresourceRange.layerCount = 1;

      if (vkCreateImageView(m_hallow_device.device(), &viewInfo, nullptr,
                            &m_swap_chain_image_views[i]) !=
          VK_SUCCESS) {
        throw std::runtime_error(
                "failed to create texture image view!");
      }
    }
  }

  void HallowSwapChain::createRenderPass() {
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = findDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};

    dependency.dstSubpass = 0;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.srcAccessMask = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    std::array<VkAttachmentDescription, 2> attachments{colorAttachment,
                                                       depthAttachment};
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(m_hallow_device.device(), &renderPassInfo, nullptr,
                           &m_render_pass) != VK_SUCCESS) {
      throw std::runtime_error("failed to create render pass!");
    }
  }

  void HallowSwapChain::createFramebuffers() {
    m_swap_chain_framebuffers.resize(imageCount());
    for (size_t i = 0; i < imageCount(); i++) {
      std::array<VkImageView, 2> attachments = {m_swap_chain_image_views[i],
                                                m_depth_image_views[i]};

      VkExtent2D swapChainExtent = swapChainExtent();
      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = m_render_pass;
      framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
      framebufferInfo.pAttachments = attachments.data();
      framebufferInfo.width = swapChainExtent.width;
      framebufferInfo.height = swapChainExtent.height;
      framebufferInfo.layers = 1;

      if (vkCreateFramebuffer(
              m_hallow_device.device(),
              &framebufferInfo,
              nullptr,
              &m_swap_chain_framebuffers[i]) != VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
      }
    }
  }

  void HallowSwapChain::createDepthResources() {
    VkFormat depthFormat = findDepthFormat();
    VkExtent2D swapChainExtent = swapChainExtent();

    m_depth_images.resize(imageCount());
    m_depth_image_memorys.resize(imageCount());
    m_depth_image_views.resize(imageCount());

    for (int i = 0; i < m_depth_images.size(); i++) {
      VkImageCreateInfo imageInfo{};
      imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
      imageInfo.imageType = VK_IMAGE_TYPE_2D;
      imageInfo.extent.width = swapChainExtent.width;
      imageInfo.extent.height = swapChainExtent.height;
      imageInfo.extent.depth = 1;
      imageInfo.mipLevels = 1;
      imageInfo.arrayLayers = 1;
      imageInfo.format = depthFormat;
      imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
      imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      imageInfo.flags = 0;

      m_hallow_device.createImageWithInfo(
              imageInfo,
              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
              m_depth_images[i],
              m_depth_image_memorys[i]);

      VkImageViewCreateInfo viewInfo{};
      viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      viewInfo.image = m_depth_images[i];
      viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      viewInfo.format = depthFormat;
      viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
      viewInfo.subresourceRange.baseMipLevel = 0;
      viewInfo.subresourceRange.levelCount = 1;
      viewInfo.subresourceRange.baseArrayLayer = 0;
      viewInfo.subresourceRange.layerCount = 1;

      if (vkCreateImageView(m_hallow_device.device(), &viewInfo, nullptr,
                            &m_depth_image_views[i]) != VK_SUCCESS) {
        throw std::runtime_error(
                "failed to create texture image view!");
      }
    }
  }

  void HallowSwapChain::createSyncObjects() {
    m_image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);
    m_images_in_flight.resize(imageCount(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      if (vkCreateSemaphore(m_hallow_device.device(), &semaphoreInfo, nullptr,
                            &m_image_available_semaphores[i]) !=
          VK_SUCCESS ||
          vkCreateSemaphore(m_hallow_device.device(), &semaphoreInfo, nullptr,
                            &m_render_finished_semaphores[i]) !=
          VK_SUCCESS ||
          vkCreateFence(m_hallow_device.device(), &fenceInfo, nullptr,
                        &m_in_flight_fences[i]) != VK_SUCCESS) {
        throw std::runtime_error(
                "failed to create synchronization objects for a frame!");
      }
    }
  }

  VkSurfaceFormatKHR HallowSwapChain::chooseSwapSurfaceFormat(
          const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
          availableFormat.colorSpace ==
          VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return availableFormat;
      }
    }

    return availableFormats[0];
  }

  VkPresentModeKHR HallowSwapChain::chooseSwapPresentMode(
          const std::vector<VkPresentModeKHR>& availablePresentModes,
          VkPresentModeKHR preferredMode) {
    for (const auto& availablePresentMode : availablePresentModes) {
      if (availablePresentMode == preferredMode) {
        std::cout << "Present mode: ";
        switch (availablePresentMode) {
          case VK_PRESENT_MODE_IMMEDIATE_KHR:std::cout << "IMMEDIATE\n";
            break;
          case VK_PRESENT_MODE_FIFO_KHR:std::cout << "FIFO (V-Sync)\n";
            break;
          case VK_PRESENT_MODE_FIFO_RELAXED_KHR:std::cout << "FIFO_RELAXED\n";
            break;
          case VK_PRESENT_MODE_MAILBOX_KHR:std::cout << "MAILBOX\n";
            break;
          case VK_PRESENT_MODE_MAX_ENUM_KHR:std::cout << "MAX_ENUM\n";
            break;
          case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:std::cout << "SHARED_CONTINUOUS_REFRESH\n";
            break;
          case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:std::cout << "SHARED_DEMAND_REFRESH\n";
            break;
        }
        return availablePresentMode;
      }
    }

    // for (const auto &availablePresentMode : availablePresentModes) {
    //   if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
    //     std::cout << "Present mode: Immediate" << std::endl;
    //     return availablePresentMode;
    //   }
    // }

    std::cerr << "Preferred mode not supported. Falling back!" << std::endl;
    std::cout << "Present mode (Fallback): FIFO (V-Sync)" << std::endl;
    return VK_PRESENT_MODE_FIFO_KHR;
  }

  VkExtent2D HallowSwapChain::chooseSwapExtent(
          const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width !=
        std::numeric_limits<uint32_t>::max()) {
      return capabilities.currentExtent;
    } else {
      VkExtent2D actualExtent = m_window_extent;
      actualExtent.width = std::max(
              capabilities.minImageExtent.width,
              std::min(capabilities.maxImageExtent.width,
                       actualExtent.width));
      actualExtent.height = std::max(
              capabilities.minImageExtent.height,
              std::min(capabilities.maxImageExtent.height,
                       actualExtent.height));

      return actualExtent;
    }
  }

  VkFormat HallowSwapChain::findDepthFormat() {
    return m_hallow_device.findSupportedFormat(
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
             VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
  }

}  // namespace Hallow
