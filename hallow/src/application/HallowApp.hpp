//
// Created by Gabriel Lugo on 3/28/2021.
//

#ifndef PROJECT_A_HALLOWAPP_H
#define PROJECT_A_HALLOWAPP_H

#include "helpers/RootDir.h"
#include "engine/time/Time.hpp"
#include "engine/window/HallowWindow.hpp"
#include "engine/device/HallowDevice.hpp"
#include "engine/game_object/HallowGameObject.hpp"
#include "engine/renderer/HallowRenderer.hpp"
#include <engine/renderer/render_system/SimpleRenderSystem.hpp>

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
    HallowApp(const HallowApp&) = delete;
    HallowApp& operator=(const HallowApp&) = delete;

    void run();
  private:
    const std::string m_name{"Hallow Engine"};

    RendererOptions m_renderer_options {
      false,
      false
    };

    Time m_time{};

    HallowWindow m_hallow_window{WIDTH, HEIGHT, m_name};
    HallowDevice m_hallow_device{m_hallow_window};
    HallowRenderer m_hallow_renderer{m_hallow_window, m_hallow_device, m_renderer_options};

    std::vector<HallowGameObject> m_game_objects;

    void loadGameObjects();

    // TODO: Move to lifetime events class
    void onStart(SimpleRenderSystem& render_system);
    void onUpdate(SimpleRenderSystem& render_system);
    void onEnd(SimpleRenderSystem& render_system);
  };
}


#endif //PROJECT_A_HALLOWAPP_H
