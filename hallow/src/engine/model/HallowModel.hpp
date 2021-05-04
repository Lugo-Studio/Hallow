//
// Created by galex on 5/4/2021.
//

#ifndef HALLOW_HALLOWMODEL_HPP
#define HALLOW_HALLOWMODEL_HPP

#include <engine/device/HallowDevice.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace Hallow {
  class HallowModel {
  public:
    struct Vertex {
      glm::vec2 position;

      static std::vector<VkVertexInputBindingDescription> bindingDescriptions();
      static std::vector<VkVertexInputAttributeDescription> attributeDescriptions();
    };

    HallowModel(HallowDevice& hallow_device, const std::vector<Vertex>& vertices);
    ~HallowModel();

    void bind(VkCommandBuffer command_buffer);
    void draw(VkCommandBuffer command_buffer);

    HallowModel(const HallowModel&) = delete;
    HallowModel& operator=(const HallowModel&) = delete;
  private:
    HallowDevice& m_hallow_device;
    VkBuffer m_vertex_buffer;
    VkDeviceMemory m_vertex_buffer_memory;
    uint32_t m_vertex_count;

    void createVertexBuffers(const std::vector<Vertex>& vertices);
  };
}


#endif //HALLOW_HALLOWMODEL_HPP
