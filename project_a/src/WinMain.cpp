//
// Created by Gabriel Lugo on 3/28/2021.
//

#include "application/HallowApp.h"


#ifdef MODE_RELEASE
#include <windows.h>
#endif

#include <cstdlib>
#include <iostream>
#include <stdexcept>


int run() {
    try {
        Hallow::HallowApp app{};
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

#ifdef MODE_RELEASE
int CALLBACK WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR     lpCmdLine,
        int       nCmdShow
        ) {
    return run();
}
#endif

#ifdef MODE_DEBUG

int main() {
    return run();
}

#endif