//
// Created by galex on 5/18/2021.
//

#ifndef HALLOW_SIMPLETRIANGLE_HPP
#define HALLOW_SIMPLETRIANGLE_HPP

#include <engine/game_object/HallowGameObject.hpp>

namespace Hallow {
  class SimpleTriangle : public HallowGameObject {
  public:
    SimpleTriangle(HallowDevice& hallow_device, bool use_srgb_color_space = false);

  private:

  };
}


#endif //HALLOW_SIMPLETRIANGLE_HPP
