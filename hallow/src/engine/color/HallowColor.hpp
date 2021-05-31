//
// Created by Gabriel Lugo on 3/31/2021.
//

#ifndef PROJECT_A_HALLOWCOLOR_H
#define PROJECT_A_HALLOWCOLOR_H

namespace Hallow {
  struct HallowColor {
    float r, g, b, a;
    bool is_linear = false;

    HallowColor(float r, float g, float b, float a = 1.0f);
    HallowColor(unsigned int hexValue);

    /*HallowColor(const HallowColor& color)
    : r{color.r}, g{color.r}, b{color.b}, a{color.a}, is_linear{color.is_linear} {};

    HallowColor& operator=(const HallowColor& color) {
      HallowColor new_color{0x000000FF};
      return {
        color.r,
        color.g,
        color.b,
        color.a,
        color.is_linear
      };
    }*/

    template<class T>
    T color_rgba() {
      T initializer_list{r, g, b, a};
      return initializer_list;
    }

    template<class T>
    T color_rgb() {
      T initializer_list{r, g, b};
      return initializer_list;
    }

    void toSrgb();
    float rSrgb();
    float gSrgb();
    float bSrgb();

    void toLinear();
    float rLinear();
    float gLinear();
    float bLinear();
  };
}
// Struct based on: https://stackoverflow.com/questions/3723846/convert-from-hex-color-to-rgb-struct-in-c

#endif //PROJECT_A_HALLOWCOLOR_H
