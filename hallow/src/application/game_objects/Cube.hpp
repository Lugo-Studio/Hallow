//
// Created by galex on 5/30/2021.
//

#ifndef HALLOW_CUBE_HPP
#define HALLOW_CUBE_HPP

// hallow
#include <engine/game_object/GameObject.hpp>

// lib

// std


namespace Hallow {
  class Cube : public GameObject {
  public:
    Cube(HallowDevice& hallow_device, glm::vec3 offset , bool use_srgb_color_space = false);

  private:
    const std::array<glm::vec3, 8> m_corners {{
                                              // front face
                                              {-.5f,-.5f,-.5f},
                                              {.5f,-.5f,-.5f},
                                              {.5f,.5f,-.5f},
                                              {-.5f,.5f,-.5f},
                                              // back face
                                              {.5f,-.5f,.5f},
                                              {-.5f,-.5f,.5f},
                                              {-.5f,.5f,.5f},
                                              {.5f,.5f,.5f}
                                            }};
  };
}


#endif //HALLOW_CUBE_HPP
