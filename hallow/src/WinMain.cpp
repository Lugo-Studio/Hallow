//
// Created by Gabriel Lugo on 3/28/2021.
//

#include "application/HallowApp.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#ifdef MODE_RELEASE
#include <windows.h>

#endif


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

int WINAPI WinMain(
    HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_cmd_show) {
  return run();
}

#endif

#ifdef MODE_DEBUG

int main() {
  return run();
}

#endif