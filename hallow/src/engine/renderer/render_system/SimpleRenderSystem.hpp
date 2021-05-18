//
// Created by galex on 5/18/2021.
//

#ifndef HALLOW_SIMPLERENDERSYSTEM_HPP
#define HALLOW_SIMPLERENDERSYSTEM_HPP

#include "helpers/RootDir.h"
#include "engine/time/Time.hpp"
#include "engine/device/HallowDevice.hpp"
#include "engine/pipeline/HallowPipeline.hpp"

#include <string>
#include <memory>
#include <vector>
#include <application/game/HallowGame.hpp>
#include <engine/game_object/HallowGameObject.hpp>


namespace Hallow {
  class SimpleRenderSystem {
  public:
    SimpleRenderSystem(Time& time, HallowDevice& device, VkRenderPass render_pass, bool m_use_srgb_color_space = false);
    ~SimpleRenderSystem();
    SimpleRenderSystem(const SimpleRenderSystem&) = delete;
    SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

    void renderGameObjects(VkCommandBuffer command_buffer, std::vector<HallowGameObject>& game_objects);
  private:
    bool m_use_srgb_color_space;
    Time& m_time;

    HallowDevice& m_hallow_device;

    std::unique_ptr<HallowPipeline> m_hallow_pipeline;
    VkPipelineLayout m_pipeline_layout;

    void createPipelineLayout();
    void createPipeline(VkRenderPass render_pass);
  };
}

#endif //HALLOW_SIMPLERENDERSYSTEM_HPP
