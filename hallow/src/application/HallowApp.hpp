//
// Created by Gabriel Lugo on 3/28/2021.
//

#ifndef PROJECT_A_HALLOWAPP_H
#define PROJECT_A_HALLOWAPP_H

#include "helpers/RootDir.h"
#include "engine/time/Time.hpp"
#include "engine/window/HallowWindow.hpp"
#include "engine/device/HallowDevice.hpp"
#include "engine/game_object/GameObject.hpp"
#include "engine/renderer/HallowRenderer.hpp"
#include <engine/renderer/render_system/RenderSystem.hpp>

#include <string>
#include <memory>
#include <vector>


namespace Hallow {
  class HallowApp {
  public:
    static constexpr int WIDTH = 1024;
    static constexpr int HEIGHT = 576;

    static bool using_srgb_color_space;
    static bool using_vsync;

    HallowApp();
    ~HallowApp();
    HallowApp(const HallowApp&) = delete;
    HallowApp& operator=(const HallowApp&) = delete;

    void run();
  private:
    const std::string m_name{"Hallow Engine"};

    Time m_time{};

    HallowWindow m_hallow_window{WIDTH, HEIGHT, m_name};
    HallowDevice m_hallow_device{m_hallow_window};
    HallowRenderer m_hallow_renderer{m_hallow_window, m_hallow_device};

    std::vector<std::shared_ptr<GameObject>> m_game_objects;

    void loadGameObjects();

    // TODO: Move to lifetime events class
    void onStart(RenderSystem& render_system);
    void onUpdate(RenderSystem& render_system);
    void onEnd(RenderSystem& render_system);
  };
}


#endif //PROJECT_A_HALLOWAPP_H
