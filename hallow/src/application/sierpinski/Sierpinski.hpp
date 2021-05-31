//
// Created by galex on 5/5/2021.
//

#ifndef HALLOW_SIERPINSKI_HPP
#define HALLOW_SIERPINSKI_HPP

#include <glm/glm.hpp>

/*namespace Hallow {
  class Sierpinski {
  public:
    struct Triangle {
      std::array<HallowModel::Vertex, 3> vertices;
    };

    static std::vector<HallowModel::Vertex> sierpinski(
      const Triangle& triangle,
      int depth);
  private:
    static float lerp(float a, float b, float t) {
      return a * (1 - t) + (b * t);
    }

    static glm::vec3 color_lerp(const glm::vec3& a, const glm::vec3& b, float t) {
      return {lerp(a[0], b[0], t),
              lerp(a[1], b[1], t),
              lerp(a[2], b[2], t)};
    }

    static HallowModel::Vertex midpoint(
      const HallowModel::Vertex& a,
      const HallowModel::Vertex& b) {
      return {{(a.position[0] + b.position[0]) * 0.5f, (a.position[1] + b.position[1]) * 0.5f},
              {color_lerp(a.color, b.color, 0.5f)}};
    }

    static Triangle midpoints(const Triangle& triangle);
    static std::array<Triangle, 3> triangle_fragments(
      const Triangle& outer_triangle,
      const Triangle& inner_triangle);
  };
}*/

#endif //HALLOW_SIERPINSKI_HPP
