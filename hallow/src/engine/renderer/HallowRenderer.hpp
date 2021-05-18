//
// Created by galex on 5/18/2021.
//

#ifndef HALLOW_HALLOWRENDERER_HPP
#define HALLOW_HALLOWRENDERER_HPP

#include "engine/time/Time.hpp"
#include "engine/window/HallowWindow.hpp"
#include "engine/device/HallowDevice.hpp"
#include "engine/swap_chain/HallowSwapChain.hpp"
#include "helpers/RootDir.h"
#include "RendererOptions.hpp"

#include <string>
#include <memory>
#include <vector>
#include <application/game/HallowGame.hpp>


namespace Hallow {
  class HallowRenderer {
  public:
    HallowRenderer(HallowWindow& window, HallowDevice& device, RendererOptions renderer_options);
    ~HallowRenderer();
    HallowRenderer(const HallowRenderer&) = delete;
    HallowRenderer& operator=(const HallowRenderer&) = delete;

    bool isFrameInProgress() const { return m_is_frame_started; }
    VkCommandBuffer currentCommandBuffer() const {
      if (!isFrameInProgress()) {
        throw std::runtime_error("HallowRenderer: Cannot get command buffer when frame not in progress!");
      }
      return m_command_buffers[m_current_frame_index];
    }
    VkRenderPass swapChainRenderPass() const { return m_hallow_swap_chain->renderPass(); }
    int frameIndex() const {
      if (!isFrameInProgress()) {
        throw std::runtime_error("HallowRenderer: Cannot get frame index when frame not in progress!");
      }
      return m_current_frame_index;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer command_buffer);
    void endSwapChainRenderPass(VkCommandBuffer command_buffer);

  private:
    RendererOptions m_renderer_options;

    HallowWindow& m_hallow_window;
    HallowDevice& m_hallow_device;
    std::unique_ptr<HallowSwapChain> m_hallow_swap_chain;
    std::vector<VkCommandBuffer> m_command_buffers;
    uint32_t m_current_image_index{0};
    int m_current_frame_index{0};
    bool m_is_frame_started{false};

    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();
  };
}


#endif //HALLOW_HALLOWRENDERER_HPP
