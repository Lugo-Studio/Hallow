//
// Created by galex on 5/20/2021.
//

#ifndef HALLOW_SIMPLERENDERSYSTEM_HPP
#define HALLOW_SIMPLERENDERSYSTEM_HPP

#include "RenderSystem.hpp"

namespace Hallow {
  class SimpleRenderSystem : public RenderSystem {
  public:
    SimpleRenderSystem(Time& time, HallowDevice& device, VkRenderPass render_pass);
    SimpleRenderSystem(const SimpleRenderSystem&) = delete;
    SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;
  private:

  };
}

#endif //HALLOW_SIMPLERENDERSYSTEM_HPP
