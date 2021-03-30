//
// Created by Gabriel Lugo on 3/28/2021.
//

#include "HallowApp.h"

#include <limits>
#include <iostream>

namespace Hallow {

    void HallowApp::run() {
        startPre();
        start();
        startPost();

        while (!m_hallow_window.shouldClose()) {
            m_time.tick(); // start with tick to get newest time
            m_time.printOnInterval(1);
            glfwPollEvents(); // poll for user events and such

            updatePre();
            update();
            updatePost();
        }

        endPre();
        end();
        endPost();
    }

    // lifetime class?
    void HallowApp::startPre() { }

    void HallowApp::start() { }

    void HallowApp::startPost() { }

    void HallowApp::updatePre() { }

    void HallowApp::update() { }

    void HallowApp::updatePost() { }

    void HallowApp::endPre() { }

    void HallowApp::end() { }

    void HallowApp::endPost() { }
}