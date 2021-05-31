//
// Created by galex on 5/5/2021.
//

#include <vector>
#include <engine/model/HallowModel.hpp>
#include <iostream>
#include "Sierpinski.hpp"

/*
namespace Hallow {

  std::vector<HallowModel::Vertex> Sierpinski::sierpinski(
    const Triangle& triangle,
    int depth) {
    std::vector<HallowModel::Vertex> vertices;
    auto inner_triangle = midpoints(triangle);

    if (depth == 0) {
      for (const auto& vert : triangle.vertices) {
        vertices.push_back(vert);
      }

      return vertices;
    } else {
      auto triangles = triangle_fragments(triangle, inner_triangle);

      --depth;
      for (const auto& tri : triangles) {
        auto verts = sierpinski(tri, depth);
        vertices.insert(vertices.end(), verts.begin(), verts.end());
      }

      return vertices;
    }
  }

  Sierpinski::Triangle Sierpinski::midpoints(const Triangle& triangle) {
    return {
      midpoint(triangle.vertices[0], triangle.vertices[1]),
      midpoint(triangle.vertices[1], triangle.vertices[2]),
      midpoint(triangle.vertices[2], triangle.vertices[0])
    };
  }

  std::array<Sierpinski::Triangle, 3> Sierpinski::triangle_fragments(
    const Triangle& outer_triangle,
    const Triangle& inner_triangle) {

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
}*/
