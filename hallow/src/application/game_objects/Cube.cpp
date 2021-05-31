//
// Created by galex on 5/30/2021.
//

// hallow
#include "Cube.hpp"

// lib

// std
#include <utility>
#include <iostream>

namespace Hallow {
  Cube::Cube(HallowDevice& hallow_device, glm::vec3 offset)
    : GameObject{currentGameObjectId()} {
    updateModel(hallow_device);

    transform().translation = offset;
    // set_color(m_face_colors[0].color_rgb<glm::vec3>());
  }

  void Cube::updateModel(HallowDevice& hallow_device) {
    generatePointMesh();
    set_model(std::make_shared<HallowModel>(hallow_device, m_vertices));
  }

  void Cube::generatePointMesh() {
    m_vertices.clear();

    // f,b,l,r,t,b
    std::array<HallowModel::Quad, 6> faces{
      {
        // quad level
        {{
           {{m_base_corners[0]},
             {m_base_corners[1]},
             {m_base_corners[2]},
             {m_base_corners[3]}}
         }, m_face_colors[0]},
        {{
           {{m_base_corners[4]},
             {m_base_corners[5]},
             {m_base_corners[6]},
             {m_base_corners[7]}}
         }, m_face_colors[1]},
        {{
           {{m_base_corners[5]},
             {m_base_corners[0]},
             {m_base_corners[3]},
             {m_base_corners[6]}}
         }, m_face_colors[2]},
        {{
           {{m_base_corners[1]},
             {m_base_corners[4]},
             {m_base_corners[7]},
             {m_base_corners[2]}}
         }, m_face_colors[3]},
        {{
           {{m_base_corners[3]},
             {m_base_corners[2]},
             {m_base_corners[7]},
             {m_base_corners[6]}}
         }, m_face_colors[4]},
        {{
           {{m_base_corners[5]},
             {m_base_corners[4]},
             {m_base_corners[1]},
             {m_base_corners[0]}}
         }, m_face_colors[5]}
      }
    };

    for (auto& quad : faces) {
      for (auto& vertex : quad.vertexVector()) {
        m_vertices.push_back(vertex);
      }
    }
  }

  void Cube::set_pointMesh(HallowDevice& hallow_device, std::array<glm::vec3, 8> base_corners) {
    m_base_corners = base_corners;

    updateModel(hallow_device);
  }

  void Cube::set_colors(HallowDevice& hallow_device,
                        std::vector<HallowColor> colors) {
    m_face_colors.clear();
    m_face_colors = std::move(colors);
    while (m_face_colors.size() < 6) {
      m_face_colors.emplace_back(0x000000FF);
    }

    updateModel(hallow_device);
    std::cerr << "set_colors not implemented for base class GameObject!\n";
  }

  void Cube::set_colors(HallowDevice& hallow_device, HallowColor color) {
    set_colors(hallow_device,
               {color, color, color, color, color, color});
  }
}