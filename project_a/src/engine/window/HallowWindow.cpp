//
// Created by Gabriel Lugo on 3/28/2021.
//

#include "HallowWindow.h"

#include <stdexcept>

namespace Hallow {

    HallowWindow::HallowWindow(int width, int height, std::string name)
            : m_width(width), m_height(height), m_name(std::move(name)), m_glfw_window(nullptr) {
        initWindow();
    }

    HallowWindow::~HallowWindow() {
        glfwDestroyWindow(m_glfw_window);
        glfwTerminate();
    }

    void HallowWindow::initWindow() {
        if (!glfwInit()) throw std::runtime_error("GLFW Error: Failed to initialize!");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Don't automagically create OpenGL context
        glfwWindowHint(GLFW_RESIZABLE, false); // Window resizing will need special treatment, so disable normal resizing

        m_glfw_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);
    }
}