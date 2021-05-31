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
      glm::vec3 position;
      glm::vec3 color;

      Vertex() : position{0.f, 0.f, 0.f}, color{1.f, 1.f, 1.f} {}

      Vertex(glm::vec3 position, glm::vec3 color)
        : position{position}, color{color} {}

      Vertex(glm::vec3 position)
        : position{position}, color{1.f, 1.f, 1.f} {}

      Vertex(float x, float y, float z)
        : position{x, y, z}, color{1.f, 1.f, 1.f} {}

      // Static functions to get size of struct and such for buffers
      static std::vector<VkVertexInputBindingDescription> bindingDescriptions();
      static std::vector<VkVertexInputAttributeDescription> attributeDescriptions();
    };

    struct Triangle {
      std::array<Vertex, 3> vertices;

      Triangle(std::array<Vertex, 3> vertices, HallowColor color)
        : vertices{vertices} {
        set_colors(color);
      }

      Triangle(std::array<Vertex, 3> vertices, std::array<HallowColor, 3> colors)
        : vertices{vertices} {
        set_colors(colors);
      }

      std::vector<Vertex> vertexVector() {
        return {vertices.begin(), vertices.end()};
      }

    private:
      void set_colors(HallowColor color) {
        for (auto& vertex : vertices) {
          vertex.color = {
            color.r,
            color.g,
            color.b
          };
        }
      }

      void set_colors(std::array<HallowColor, 3> colors) {
        for (int i = 0; i < vertices.size(); ++i) {
          vertices[i].color = {
            colors[i].r,
            colors[i].g,
            colors[i].b
          };
        }
      }
    };

    struct Quad {
      /* vertex order:
       * 3 2
       * 0 1
       *
       * triangles formed:
       * (0 1 2)
       * (0 2 3)
       */
      std::array<Vertex, 4> vertices;

      Quad(std::array<Vertex, 4> vertices, HallowColor color)
        : vertices{vertices} {
        set_colors(color);
      }

      Quad(std::array<Vertex, 4> vertices, std::array<HallowColor, 4> colors)
        : vertices{vertices} {
        set_colors(colors);
      }

      std::vector<Vertex> vertexVector() {
        return {
          vertices[0],
          vertices[1],
          vertices[2],
          vertices[0],
          vertices[2],
          vertices[3]
        };
      }
    private:
      void set_colors(HallowColor color) {
        for (auto& vertex : vertices) {
          vertex.color = {
            color.r,
            color.g,
            color.b
          };
        }
      }

      void set_colors(std::array<HallowColor, 4> colors) {
        for (int i = 0; i < vertices.size(); ++i) {
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
