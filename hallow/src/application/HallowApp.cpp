//
// Created by Gabriel Lugo on 3/28/2021.
//

#include "HallowApp.hpp"
#include <application/game_objects/SimpleTriangle.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <stdexcept>
#include <array>


namespace Hallow {
  HallowApp::HallowApp() {
    loadGameObjects();
  }

  HallowApp::~HallowApp() {}

  void HallowApp::run() {
    SimpleRenderSystem simple_render_system{m_time,
                                            m_hallow_device,
                                            m_hallow_renderer.swapChainRenderPass(),
                                            m_renderer_options};

    onStart(simple_render_system);

    while (!m_hallow_window.shouldClose()) {
      onUpdate(simple_render_system);
    }

    onEnd(simple_render_system);
  }

  void HallowApp::onStart(SimpleRenderSystem& render_system) {
    // Pre-start
    m_hallow_game.onPreStart();

    // Start
    m_hallow_game.onStart();

    // Post-start
    m_hallow_game.onPostStart();
  }

  void HallowApp::onUpdate(SimpleRenderSystem& render_system) {
    // Pre-update
    m_hallow_game.onPreUpdate();

    // Update
    m_time.tick(); // onStart with tick to get newest time
    //m_time.printOnInterval(1);
    glfwPollEvents(); // poll for user events and such
    m_hallow_game.onUpdate();


    // Post-update
    // will return nullptr if swapchain needs to be recreated
    if (auto command_buffer = m_hallow_renderer.beginFrame()) {
      m_hallow_renderer.beginSwapChainRenderPass(command_buffer);
      render_system.renderGameObjects(command_buffer, m_game_objects);
      m_hallow_renderer.endSwapChainRenderPass(command_buffer);
      m_hallow_renderer.endFrame();
    }
    m_hallow_game.onPostUpdate();
  }

  void HallowApp::onEnd(SimpleRenderSystem& render_system) {
    // Pre-end
    m_hallow_game.onPreEnd();

    // End
    vkDeviceWaitIdle(m_hallow_device.device());
    m_hallow_game.onEnd();

    // Post-end
    m_hallow_game.onPostEnd();
  }

  void HallowApp::loadGameObjects() {
    //sierpinskiModel();
    SimpleTriangle triangle_object{m_hallow_device, m_renderer_options.using_srgb_color_space};

    m_game_objects.push_back(std::move(triangle_object));
  }
}
