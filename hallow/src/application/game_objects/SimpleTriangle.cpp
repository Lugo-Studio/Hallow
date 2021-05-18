//
// Created by galex on 5/18/2021.
//

#include "SimpleTriangle.hpp"

namespace Hallow {
  SimpleTriangle::SimpleTriangle(Hallow::HallowDevice& hallow_device, bool use_srgb_color_space)
    : HallowGameObject{currentGameObjectId()} {
    HallowColor triangle_color{0xBF616AFF, use_srgb_color_space};
    HallowModel::Triangle triangle{
      {{{-0.5f, 0.5f},
        {0.5f, 0.5f},
        {0.0f, -0.5f}}},
      triangle_color
    };
    auto m_hallow_model = std::make_shared<HallowModel>(hallow_device, triangle.vertexVector());

    set_model(m_hallow_model);
    set_color(triangle_color.color_rgb<glm::vec3>());
  }
}