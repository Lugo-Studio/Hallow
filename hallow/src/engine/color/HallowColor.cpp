//
// Created by galex on 5/31/2021.
//

// hallow
#include "HallowColor.hpp"
#include <application/HallowApp.hpp>
#include <engine/color/srgb_transform/SrgbTransform.hpp>

// std
#include <glm/glm.hpp>

namespace Hallow {
  HallowColor::HallowColor(float r, float g, float b, float a)
    : r(r), g(g), b(b), a(a) {
    if (HallowApp::using_srgb_color_space) {
      toLinear();
    }
  }

  HallowColor::HallowColor(unsigned int hexValue) {
    // RRGGBBAA
    r = static_cast<float>((hexValue >> 24) & 0xFF) / 255.0f;  // Extract the RR byte
    g = static_cast<float>((hexValue >> 16) & 0xFF) / 255.0f;   // Extract the GG byte
    b = static_cast<float>((hexValue >> 8) & 0xFF) / 255.0f;   // Extract the BB byte
    a = static_cast<float>((hexValue) & 0xFF) / 255.0f;   // Extract the AA byte

    if (HallowApp::using_srgb_color_space) {
      toLinear();
    }
  }

  void HallowColor::toLinear() {
    if (!is_linear) {
      r = rLinear();
      g = gLinear();
      b = bLinear();
      is_linear = true;
    }
  }

  void HallowColor::toSrgb() {
    if (is_linear) {
      r = rSrgb();
      g = gSrgb();
      b = bSrgb();
      is_linear = false;
    }
  }

  float HallowColor::rSrgb() { return linearToSrgb(r); }
  float HallowColor::gSrgb() { return linearToSrgb(g); }
  float HallowColor::bSrgb() { return linearToSrgb(b); }
  float HallowColor::rLinear() { return srgbToLinear(r); }
  float HallowColor::gLinear() { return srgbToLinear(g); }
  float HallowColor::bLinear() { return srgbToLinear(b); }
}

