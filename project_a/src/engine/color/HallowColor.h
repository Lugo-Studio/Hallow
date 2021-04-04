//
// Created by Gabriel Lugo on 3/31/2021.
//

#ifndef PROJECT_A_HALLOWCOLOR_H
#define PROJECT_A_HALLOWCOLOR_H

namespace Hallow {
    struct Color {
        float r, g, b, a;

        Color(float r, float g, float b, float a)
                : r(r), g(g), b(b), a(a) {}

        explicit Color(unsigned int hexValue) {
            // RRGGBBAA
            r = static_cast<float>((hexValue >> 24) & 0xFF) /
                255.0f;  // Extract the RR byte
            g = static_cast<float>((hexValue >> 16) & 0xFF) /
                255.0f;   // Extract the GG byte
            b = static_cast<float>((hexValue >> 8) & 0xFF) /
                255.0f;   // Extract the BB byte
            a = static_cast<float>((hexValue) & 0xFF) /
                255.0f;   // Extract the AA byte
        }
    };
}
// Struct based on: https://stackoverflow.com/questions/3723846/convert-from-hex-color-to-rgb-struct-in-c

#endif //PROJECT_A_HALLOWCOLOR_H
