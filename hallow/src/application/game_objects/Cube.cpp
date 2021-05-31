//
// Created by galex on 5/30/2021.
//

// hallow
#include "Cube.hpp"

// lib

// std

namespace Hallow {
  Cube::Cube(HallowDevice& hallow_device, glm::vec3 offset , bool use_srgb_color_space)
    : GameObject{currentGameObjectId()} {
    // f,b,l,r,t,b
    std::array<HallowColor, 6> face_colors{{
                                             {0xBF616AFF, use_srgb_color_space},
                                             {0xD08770FF, use_srgb_color_space},
                                             {0xEBCB8BFF, use_srgb_color_space},
                                             {0xA3BE8CFF, use_srgb_color_space},
                                             {0x88C0D0FF, use_srgb_color_space},
                                             {0xB48EADFF, use_srgb_color_space}
                                           }};

    // f,b,l,r,t,b
    std::array<HallowModel::Quad, 6> faces{
      {
        // quad level
        {{
          {{m_corners[0]},
            {m_corners[1]},
            {m_corners[2]},
            {m_corners[3]}}
        }, face_colors[0]},
        {{
           {{m_corners[4]},
             {m_corners[5]},
             {m_corners[6]},
             {m_corners[7]}}
         }, face_colors[1]},
        {{
           {{m_corners[5]},
             {m_corners[0]},
             {m_corners[3]},
             {m_corners[6]}}
         }, face_colors[2]},
        {{
           {{m_corners[1]},
             {m_corners[4]},
             {m_corners[7]},
             {m_corners[2]}}
         }, face_colors[3]},
        {{
           {{m_corners[3]},
             {m_corners[2]},
             {m_corners[7]},
             {m_corners[6]}}
         }, face_colors[4]},
        {{
           {{m_corners[5]},
             {m_corners[4]},
             {m_corners[1]},
             {m_corners[0]}}
         }, face_colors[5]}
      }
    };

    std::vector<HallowModel::Vertex> vertices{8*6};
    for (auto& quad : faces) {
      for (auto& vertex : quad.vertexVector()) {
        vertex.position += offset;
        vertices.push_back(vertex);
      }
    }

    auto m_hallow_model = std::make_shared<HallowModel>(hallow_device, vertices);

    set_model(m_hallow_model);
    // set_color(face_colors[0].color_rgb<glm::vec3>());
  }
}