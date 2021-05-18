//
// Created by galex on 5/4/2021.
//

#ifndef HALLOW_HALLOWMODEL_HPP
#define HALLOW_HALLOWMODEL_HPP

#include <engine/device/HallowDevice.hpp>
#include <engine/color/HallowColor.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>
#include <array>

namespace Hallow {
  class HallowModel {
  public:
    struct Vertex {
      glm::vec2 position;
      glm::vec3 color;

      Vertex() : position{0.0f, 0.0f}, color{1.0f, 1.0f, 1.0f} {}

      Vertex(glm::vec2 position, glm::vec3 color)
        : position(position), color(color) {}

      explicit Vertex(glm::vec2 position)
        : position(position), color{1.0f, 1.0f, 1.0f} {}

      Vertex(float p1, float p2)
        : position{p1, p2}, color{1.0f, 1.0f, 1.0f} {}

      // Static functions to get size of struct and such for buffers
      static std::vector<VkVertexInputBindingDescription> bindingDescriptions();
      static std::vector<VkVertexInputAttributeDescription> attributeDescriptions();
    };

    struct Triangle {
      std::array<Vertex, 3> vertices;

      Triangle(std::array<Vertex, 3> vertices, HallowColor color)
        : vertices(vertices) {
        set_colors({color, color, color});
      }

      Triangle(std::array<Vertex, 3> vertices, std::array<HallowColor, 3> colors)
        : vertices(vertices) {
        set_colors(colors);
      }

      std::vector<Vertex> vertexVector();

    private:
      void set_colors(std::array<HallowColor, 3> colors) {
        for (int i = 0; i < vertices.size(); ++i) {
          /*if (m_hallow_swap_chain.usingSrgbColorSpace()) {
            background_color.toSrgb();
          }*/
          vertices[i].color = {
            colors[i].r,
            colors[i].g,
            colors[i].b
          };
        }
      }
    };

    HallowModel(HallowDevice& hallow_device, const std::vector<Vertex>& vertices);
    ~HallowModel();
    HallowModel(const HallowModel&) = delete;
    HallowModel& operator=(const HallowModel&) = delete;

    void bind(VkCommandBuffer command_buffer);
    void draw(VkCommandBuffer command_buffer);
  private:
    HallowDevice& m_hallow_device;
    VkBuffer m_vertex_buffer;
    VkDeviceMemory m_vertex_buffer_memory;
    uint32_t m_vertex_count;

    void createVertexBuffers(const std::vector<Vertex>& vertices);
  };
}


#endif //HALLOW_HALLOWMODEL_HPP
