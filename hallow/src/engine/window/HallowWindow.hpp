//
// Created by Gabriel Lugo on 3/28/2021.
//

#ifndef PROJECT_A_HALLOWWINDOW_H
#define PROJECT_A_HALLOWWINDOW_H

#define GLFW_INCLUDE_VULKAN

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <string>
#include <utility>
#include <memory>


namespace Hallow {
  class HallowWindow {
  public:
    HallowWindow(int width, int height, std::string name);
    ~HallowWindow();
    HallowWindow(const HallowWindow&) = delete;
    HallowWindow& operator=(const HallowWindow&) = delete;

    bool shouldClose() { return glfwWindowShouldClose(m_glfw_window); }
    bool wasWindowResized() { return m_framebuffer_resized; }
    void resetResizedFlag() { m_framebuffer_resized = false; }

    VkExtent2D extent() {
      return {
        static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height)
      };
    }

    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
  private:
    int m_width;
    int m_height;
    std::string m_name;
    bool m_framebuffer_resized{false};

    GLFWwindow* m_glfw_window;

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void windowRefreshCallback(GLFWwindow* window);

    void initWindow();
  };
}


#endif //PROJECT_A_HALLOWWINDOW_H
