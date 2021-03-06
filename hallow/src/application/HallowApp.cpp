//
// Created by Gabriel Lugo on 3/28/2021.
//

// hallow
#include "HallowApp.hpp"
#include <application/game_objects/SimpleTriangle.hpp>
#include <engine/renderer/render_system/SimpleRenderSystem.hpp>
#include <application/game_objects/Cube.hpp>

// lib
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <iostream>
#include <stdexcept>
#include <array>


namespace Hallow {
  bool HallowApp::using_srgb_color_space = false;
  bool HallowApp::using_vsync = false;

  HallowApp::HallowApp() {
    std::cout << "[HallowApp] Vsync: " << HallowApp::using_vsync << "\n";
    std::cout << "[HallowApp] Vsync: " << HallowApp::using_vsync << "\n";

    loadGameObjects();
  }

  HallowApp::~HallowApp() = default;

  void HallowApp::run() {
    // Initilization
    SimpleRenderSystem simple_render_system{m_time,
                                            m_hallow_device,
                                            m_hallow_renderer.swapChainRenderPass()};
    simple_render_system.initilizePipeline();

    // Run start
    onStart(simple_render_system);
    // Run loop
    while (!m_hallow_window.shouldClose()) {
      onUpdate(simple_render_system);
    }
    // Run end
    onEnd(simple_render_system);
  }

  void HallowApp::onStart(RenderSystem& render_system) {

  }

  void HallowApp::onUpdate(RenderSystem& render_system) {
    // Update
    m_time.tick(); // onStart with tick to get newest time
    //m_time.printOnInterval(1);
    glfwPollEvents(); // poll for user events and such

    static unsigned int color_base = 0x000000FF;

    // m_game_objects[0]->set_colors(m_hallow_device, color_base);

    if (color_base < 0xFFFFFFFF) {
      ++color_base;
    } else {
      color_base = 0x000000FF;
    }

    // will return nullptr if swapchain needs to be recreated
    if (auto command_buffer = m_hallow_renderer.beginFrame()) {
      m_hallow_renderer.beginSwapChainRenderPass(command_buffer);

      render_system.renderGameObjects(command_buffer, m_game_objects);
      m_hallow_renderer.endSwapChainRenderPass(command_buffer);
      m_hallow_renderer.endFrame();
    }

    // Post-update
  }

  void HallowApp::onEnd(RenderSystem& render_system) {
    vkDeviceWaitIdle(m_hallow_device.device());
  }

  void HallowApp::loadGameObjects() {
    //sierpinskiModel();
    glm::vec3 cube_pos = {0.f, 0.f, .5f};
    auto cube = std::make_shared<Cube>(m_hallow_device, cube_pos);
    cube->transform().scale = {.5f, .5f, .5f};

    m_game_objects.push_back(std::move(cube));
  }
}
