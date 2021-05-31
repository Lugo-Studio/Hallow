//
// Created by Gabriel Lugo on 3/31/2021.
//

#ifndef PROJECT_A_HALLOWCOLOR_H
#define PROJECT_A_HALLOWCOLOR_H

#include <engine/color/srgb_transform/SrgbTransform.hpp>
#include <glm/glm.hpp>

namespace Hallow {
  struct HallowColor {
    float r, g, b, a;
    bool linear = false;

    HallowColor(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f, bool is_color_space_srgb = false)
      : r(r), g(g), b(b), a(a) {
      if (is_color_space_srgb) {
        toLinear();
      }
    }

    HallowColor(unsigned int hexValue = 0x00000000, bool is_color_space_srgb = false) {
      // RRGGBBAA
      r = static_cast<float>((hexValue >> 24) & 0xFF) / 255.0f;  // Extract the RR byte
      g = static_cast<float>((hexValue >> 16) & 0xFF) / 255.0f;   // Extract the GG byte
      b = static_cast<float>((hexValue >> 8) & 0xFF) / 255.0f;   // Extract the BB byte
      a = static_cast<float>((hexValue) & 0xFF) / 255.0f;   // Extract the AA byte

      if (is_color_space_srgb) {
        toLinear();
      }
    }

    void toLinear() {
      if (!linear) {
        r = rLinear();
        g = gLinear();
        b = bLinear();
        linear = true;
      }
    }

    void toSrgb() {
      if (linear) {
        r = rSrgb();
        g = gSrgb();
        b = bSrgb();
        linear = false;
      }
    }

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

    float rSrgb() { return linearToSrgb(r); }

    float gSrgb() { return linearToSrgb(g); }

    float bSrgb() { return linearToSrgb(b); }

    float rLinear() { return srgbToLinear(r); }

    float gLinear() { return srgbToLinear(g); }

    float bLinear() { return srgbToLinear(b); }
  };
}
// Struct based on: https://stackoverflow.com/questions/3723846/convert-from-hex-color-to-rgb-struct-in-c

#endif //PROJECT_A_HALLOWCOLOR_H
