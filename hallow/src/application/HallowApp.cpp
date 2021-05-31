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
  HallowApp::HallowApp() {
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


    // Post-update
    // will return nullptr if swapchain needs to be recreated
    if (auto command_buffer = m_hallow_renderer.beginFrame()) {
      m_hallow_renderer.beginSwapChainRenderPass(command_buffer);
      render_system.renderGameObjects(command_buffer, m_game_objects);
      m_hallow_renderer.endSwapChainRenderPass(command_buffer);
      m_hallow_renderer.endFrame();
    }
  }

  void HallowApp::onEnd(RenderSystem& render_system) {
    vkDeviceWaitIdle(m_hallow_device.device());
  }

  void HallowApp::loadGameObjects() {
    //sierpinskiModel();
    Cube cube{m_hallow_device,
              {0.f, 0.f, 0.f},
              m_renderer_options.using_srgb_color_space};
    cube.transform().translation = {0.f, 0.f, .5f};
    cube.transform().scale = {.5f, .5f, .5f};

    m_game_objects.push_back(std::move(cube));
  }
}
