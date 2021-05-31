//
// Created by galex on 5/18/2021.
//

#ifndef HALLOW_RENDERSYSTEM_HPP
#define HALLOW_RENDERSYSTEM_HPP

#include "helpers/RootDir.h"
#include "engine/time/Time.hpp"
#include "engine/device/HallowDevice.hpp"
#include "engine/pipeline/HallowPipeline.hpp"

#include <string>
#include <memory>
#include <vector>
#include <engine/game_object/GameObject.hpp>


namespace Hallow {
  class RenderSystem {
  public:
    RenderSystem(Time& time, HallowDevice& device, VkRenderPass render_pass);
    virtual ~RenderSystem();
    RenderSystem(const RenderSystem&) = delete;
    RenderSystem& operator=(const RenderSystem&) = delete;

    virtual void initilizePipeline();
    virtual void renderGameObjects(VkCommandBuffer command_buffer, std::vector<std::shared_ptr<GameObject>>& game_objects);

  protected:
    // RendererOptions m_renderer_options;
    Time& m_time;
    const std::string m_shader_path{"res/shaders/simple_shader"};

    HallowDevice& m_hallow_device;

    VkRenderPass m_render_pass;
    std::unique_ptr<HallowPipeline> m_hallow_pipeline;
    VkPipelineLayout m_pipeline_layout;

    bool m_pipeline_created{false};

    virtual void createPipelineLayout();
    virtual void createPipeline(VkRenderPass render_pass);
  };
}

#endif //HALLOW_RENDERSYSTEM_HPP
