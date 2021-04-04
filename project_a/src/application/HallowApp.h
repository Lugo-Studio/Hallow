//
// Created by Gabriel Lugo on 3/28/2021.
//

#ifndef PROJECT_A_HALLOWAPP_H
#define PROJECT_A_HALLOWAPP_H

#include "engine/time/Time.h"
#include "engine/window/HallowWindow.h"
#include "engine/device/HallowDevice.h"
#include "engine/swap_chain/HallowSwapChain.h"
#include "engine/pipeline/HallowPipeline.h"
#include "helpers/RootDir.h"

#include <string>
#include <memory>
#include <vector>


namespace Hallow {
  class HallowApp {
  public:
    static constexpr int WIDTH = 1024;
    static constexpr int HEIGHT = 576;

    HallowApp();
    ~HallowApp();

    void run();

    HallowApp(const HallowApp&) = delete;
    HallowApp& operator=(const HallowApp&) = delete;
  private:
    const std::string m_name{"Hallow Engine"};

    Time m_time{};
    HallowWindow m_hallow_window{WIDTH, HEIGHT, m_name};
    HallowDevice m_hallow_device{m_hallow_window};
    HallowSwapChain m_hallow_swap_chain{m_hallow_device,
                                        m_hallow_window.extent()};
    std::unique_ptr<HallowPipeline> m_hallow_pipeline;
    VkPipelineLayout m_pipeline_layout;
    std::vector<VkCommandBuffer> m_command_buffers;

    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();

    /*HallowPipeline m_hallow_pipeline{
        m_hallow_device,
        HallowPipeline::defaultPipelineConfig(WIDTH, HEIGHT),
        "res/shaders/simple_shader"
    };*/

    // TODO: Move to lifetime events class
    void startPre();
    void start();
    void startPost();

    void updatePre();
    void update();
    void updatePost();

    void endPre();
    void end();
    void endPost();
  };
}


#endif //PROJECT_A_HALLOWAPP_H
