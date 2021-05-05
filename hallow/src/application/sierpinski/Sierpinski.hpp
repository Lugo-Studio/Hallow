//
// Created by galex on 5/5/2021.
//

#ifndef HALLOW_SIERPINSKI_HPP
#define HALLOW_SIERPINSKI_HPP

namespace Hallow {
  class Sierpinski {
  public:
    static std::vector<HallowModel::Vertex> sierpinski(
        const HallowModel::Triangle& triangle,
        int depth);
  private:
    static HallowModel::Vertex midpoint(
        const glm::vec2& v1,
        const glm::vec2& v2) {
      return {{(v1[0] + v2[0]) * 0.5f, (v1[1] + v2[1]) * 0.5f}};
    }

    static HallowModel::Triangle midpoints(const HallowModel::Triangle& triangle);
    static std::array<HallowModel::Triangle, 3> smaller_triangles(
        const HallowModel::Triangle& outer_triangle,
        const HallowModel::Triangle& inner_triangle);
  };
}

#endif //HALLOW_SIERPINSKI_HPP
