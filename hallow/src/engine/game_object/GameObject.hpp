//
// Created by galex on 5/18/2021.
//

#ifndef HALLOW_GAMEOBJECT_HPP
#define HALLOW_GAMEOBJECT_HPP

// hallow
#include <engine/model/HallowModel.hpp>

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>

namespace Hallow {
  struct Transform {
    glm::vec3 translation{0.f, 0.f, 0.f}; // (position offset)
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation{0.f, 0.f, 0.f};

    // glm works with array rows mapping to matrix columns, not rows!
    // think of it as each array row being a basis vector

    /* Homogeneous coordinates:
     * / a b c tx \
     * | e f g ty |
     * | h i j tz |
     * \ 0 0 0 1  /
     * t coordinates are the translation vector components.
     * 1 in the 4th component for a regular vector (not the 4x4 matrix) means position vector, 0 means direction.
     * This means that when the vector is multiplied by the matrix,
     * directions will cancel out the translation offset components.
     */

    // matrix corresponds to translate * ry * rx * rz * scale transform
    // rotation convention uses tait-bryan angles with axis order y(1), x(2), z(3)
    glm::mat4 mat4_slow() {
      // create identity matrix with t coordinates filled by the translation vector
      auto transform = glm::translate(glm::mat4{1.f}, translation);

      transform = glm::rotate(transform, rotation.y, {0.f, 1.f, 0.f});
      transform = glm::rotate(transform, rotation.x, {1.f, 0.f, 0.f});
      transform = glm::rotate(transform, rotation.z, {0.f, 0.f, 1.f});

      transform = glm::scale(transform, scale);

      return transform;
    }

    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    glm::mat4 mat4() {
      const float c3 = glm::cos(rotation.z);
      const float s3 = glm::sin(rotation.z);
      const float c2 = glm::cos(rotation.x);
      const float s2 = glm::sin(rotation.x);
      const float c1 = glm::cos(rotation.y);
      const float s1 = glm::sin(rotation.y);
      return glm::mat4{
        {scale.x * (c1 * c3 + s1 * s2 * s3),
                        scale.x * (c2 * s3),
                                       scale.x * (c1 * s2 * s3 - c3 * s1),
                                                      0.f},
        {scale.y * (c3 * s1 * s2 - c1 * s3),
                        scale.y * (c2 * c3),
                                       scale.y * (c1 * c3 * s2 + s1 * s3),
                                                      0.f},
        {scale.z * (c2 * s1),
                        scale.z * (-s2),
                                       scale.z * (c1 * c2),
                                                      0.f},
        {translation.x, translation.y, translation.z, 1.f}};
    }
  };

  class GameObject {
  public:
    using id_t = unsigned int;

    static GameObject createGameObject() {
      return GameObject{currentGameObjectId()};
    }

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    GameObject(GameObject&&) = default;
    GameObject& operator=(GameObject&&) = default;

    id_t id() const { return m_id; }
    Transform& transform() { return m_transform; }
    void set_transform(Transform transform) { m_transform = transform; }
    std::shared_ptr<HallowModel>& model() { return m_model; }
    void set_model(std::shared_ptr<HallowModel> model) { m_model = model; }
    // glm::vec3& color() { return m_color; }
    // void set_color(glm::vec3 color) { m_color = color; }

  protected:
    id_t m_id;

    // Components
    Transform m_transform{};
    std::shared_ptr<HallowModel> m_model{};
    // glm::vec3 m_color{};

    GameObject(id_t object_id) : m_id(object_id) {}

    static id_t currentGameObjectId() {
      static id_t current_id = 0;
      return current_id++;
    }
  };
}


#endif //HALLOW_GAMEOBJECT_HPP
