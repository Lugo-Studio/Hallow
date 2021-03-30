//
// Created by Gabriel Lugo on 3/28/2021.
//

#include "application/HallowApp.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
    Hallow::HallowApp app{};

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}