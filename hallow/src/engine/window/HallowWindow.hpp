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

    bool shouldClose() { return glfwWindowShouldClose(m_glfw_window); }

    VkExtent2D extent() {
      return {
          static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height)
      };
    }

    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

    // no copying allowed
    HallowWindow(const HallowWindow&) = delete;
    HallowWindow& operator=(const HallowWindow&) = delete;
  private:
    const int m_width;
    const int m_height;
    std::string m_name;

    GLFWwindow* m_glfw_window;

    void initWindow();
  };
}


#endif //PROJECT_A_HALLOWWINDOW_H
