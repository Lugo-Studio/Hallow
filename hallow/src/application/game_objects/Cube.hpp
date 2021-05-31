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
    Cube(HallowDevice& hallow_device, glm::vec3 offset);

    void set_pointMesh(HallowDevice& hallow_device, std::array<glm::vec3, 8> base_corners);

    void set_colors(HallowDevice& hallow_device, std::vector<HallowColor> colors) override;
    void set_colors(HallowDevice& hallow_device, HallowColor color) override;
  private:
    void updateModel(HallowDevice& hallow_device);
    void generatePointMesh();

    std::array<glm::vec3, 8> m_base_corners {{
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

    // f,b,l,r,t,b
    std::vector<HallowColor> m_face_colors {{
                                              {0xBF616AFF},
                                              {0xD08770FF},
                                              {0xEBCB8BFF},
                                              {0xA3BE8CFF},
                                              {0x88C0D0FF},
                                              {0xB48EADFF}
                                            }};
  };
}


#endif //HALLOW_CUBE_HPP
