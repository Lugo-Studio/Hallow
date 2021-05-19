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
    // Initilization
    SimpleRenderSystem simple_render_system{m_time,
                                            m_hallow_device,
                                            m_hallow_renderer.swapChainRenderPass()};

    // Run start
    onStart(simple_render_system);
    // Run loop
    while (!m_hallow_window.shouldClose()) {
      onUpdate(simple_render_system);
    }
    // Run end
    onEnd(simple_render_system);
  }

  void HallowApp::onStart(SimpleRenderSystem& render_system) {

  }

  void HallowApp::onUpdate(SimpleRenderSystem& render_system) {
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

  void HallowApp::onEnd(SimpleRenderSystem& render_system) {
    vkDeviceWaitIdle(m_hallow_device.device());
  }

  void HallowApp::loadGameObjects() {
    //sierpinskiModel();
    SimpleTriangle triangle_object{m_hallow_device, m_renderer_options.using_srgb_color_space};

    m_game_objects.push_back(std::move(triangle_object));
  }
}
