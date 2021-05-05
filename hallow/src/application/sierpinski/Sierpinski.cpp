//
// Created by galex on 5/5/2021.
//

#include <vector>
#include <engine/model/HallowModel.hpp>
#include <iostream>
#include "Sierpinski.hpp"

namespace Hallow {

  std::vector<HallowModel::Vertex> Sierpinski::sierpinski(
      const HallowModel::Triangle& triangle,
      int depth) {
    std::vector<HallowModel::Vertex> vertices;
    auto inner_triangle = midpoints(triangle);

    if (depth == 0) {
      for (const auto& vert : triangle.vertices) {
        vertices.push_back(vert);
      }

      return vertices;
    } else {
      auto triangles = smaller_triangles(triangle, inner_triangle);

      --depth;
      for (const auto& tri : triangles) {
        auto verts = sierpinski(tri, depth);
        vertices.insert(vertices.end(), verts.begin(), verts.end());
      }

      return vertices;
    }
  }

  HallowModel::Triangle Sierpinski::midpoints(const HallowModel::Triangle& triangle) {
    return {
        midpoint(triangle.vertices[0].position, triangle.vertices[1].position),
        midpoint(triangle.vertices[1].position, triangle.vertices[2].position),
        midpoint(triangle.vertices[2].position, triangle.vertices[0].position)
    };
  }

  std::array<HallowModel::Triangle, 3> Sierpinski::smaller_triangles(
      const HallowModel::Triangle& outer_triangle,
      const HallowModel::Triangle& inner_triangle) {
    return {{
                {outer_triangle.vertices[0],
                    inner_triangle.vertices[0],
                    inner_triangle.vertices[2]},
                {inner_triangle.vertices[0],
                    outer_triangle.vertices[1],
                    inner_triangle.vertices[1]},
                {inner_triangle.vertices[2],
                    inner_triangle.vertices[1],
                    outer_triangle.vertices[2]}
            }};
  }
}