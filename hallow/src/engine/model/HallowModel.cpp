//
// Created by galex on 5/4/2021.
//

#include <stdexcept>
#include "HallowModel.hpp"

namespace Hallow {
  HallowModel::HallowModel(HallowDevice& hallow_device, const std::vector<Vertex>& vertices) : m_hallow_device(
      hallow_device) {
    createVertexBuffers(vertices);
  }

  HallowModel::~HallowModel() {
    vkDestroyBuffer(m_hallow_device.device(), m_vertex_buffer, nullptr);
    vkFreeMemory(m_hallow_device.device(), m_vertex_buffer_memory, nullptr);
  }

  void HallowModel::bind(VkCommandBuffer command_buffer) {
    VkBuffer buffers[] = {m_vertex_buffer};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(command_buffer, 0, 1, buffers, offsets);
  }

  void HallowModel::draw(VkCommandBuffer command_buffer) {
    vkCmdDraw(command_buffer, m_vertex_count, 1, 0, 0);
  }

  void HallowModel::createVertexBuffers(const std::vector<Vertex>& vertices) {
    m_vertex_count = static_cast<uint32_t>(vertices.size());
    if (m_vertex_count < 3) {
      throw std::runtime_error("HallowModel: Vertex count must be at least 3 (at least one triangle)");
    }

    VkDeviceSize buffer_size = sizeof(vertices[0]) * m_vertex_count;
    m_hallow_device.createBuffer(buffer_size,
                                 VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                 m_vertex_buffer,
                                 m_vertex_buffer_memory);

    void* data;
    vkMapMemory(m_hallow_device.device(), m_vertex_buffer_memory, 0, buffer_size, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(buffer_size));
    vkUnmapMemory(m_hallow_device.device(), m_vertex_buffer_memory);
  }

  std::vector<VkVertexInputBindingDescription> HallowModel::Vertex::bindingDescriptions() {
    return {{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}};
  }

  std::vector<VkVertexInputAttributeDescription> HallowModel::Vertex::attributeDescriptions() {
    return {{0, 0, VK_FORMAT_R32G32_SFLOAT, 0}};
  }
}