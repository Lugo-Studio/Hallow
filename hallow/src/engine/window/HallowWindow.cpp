//
// Created by Gabriel Lugo on 3/28/2021.
//

#include "HallowWindow.hpp"

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
    if (!glfwInit()) {
      throw std::runtime_error("GLFW Error: Failed to initialize!");
    }
    // Don't automagically create OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_RESIZABLE, true);
    // glfwWindowHint(GLFW_DECORATED, false);

    m_glfw_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(m_glfw_window, this);
    glfwSetFramebufferSizeCallback(m_glfw_window, framebufferResizeCallback);
    glfwSetWindowRefreshCallback(m_glfw_window, windowRefreshCallback);
  }

  void HallowWindow::createWindowSurface(
    VkInstance instance, VkSurfaceKHR* surface) {
    if (glfwCreateWindowSurface(instance, m_glfw_window, nullptr, surface) != VK_SUCCESS) {
      throw std::runtime_error("GLFW Error: Failed to create m_hallow_window surface!");
    }
  }

  void HallowWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto hallow_window = reinterpret_cast<HallowWindow*>(glfwGetWindowUserPointer(window));
    hallow_window->m_framebuffer_resized = true;
    hallow_window->m_width = width;
    hallow_window->m_height = height;
  }

  void HallowWindow::windowRefreshCallback(GLFWwindow* window) {
    auto hallow_window = reinterpret_cast<HallowWindow*>(glfwGetWindowUserPointer(window));

  }
}