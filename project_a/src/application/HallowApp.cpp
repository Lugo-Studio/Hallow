//
// Created by Gabriel Lugo on 3/28/2021.
//

#include "HallowApp.h"

#include <limits>
#include <iostream>

namespace Hallow {

    void HallowApp::run() {
        start();

        while (!m_hallow_window.shouldClose()) {
            m_time.tick(); // start with tick to get newest time
            m_time.printOnInterval(1);
            glfwPollEvents(); // poll for user events and such

            update();
        }
    }

    void HallowApp::start() {
    }

    void HallowApp::update() {

    }

    void HallowApp::fixedUpdate() {

    }
}