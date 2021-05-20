//
// Created by galex on 5/20/2021.
//

#include "SimpleRenderSystem.hpp"

namespace Hallow {
  /*struct SimplePushConstantData {
    alignas(16) glm::mat2 transform{1.0f};
    alignas(8) glm::vec2 offset;
    alignas(16) glm::vec3 color;
  };*/

  SimpleRenderSystem::SimpleRenderSystem(Time& time, HallowDevice& device, VkRenderPass render_pass)
    : RenderSystem(time, device, render_pass) {
  }
}