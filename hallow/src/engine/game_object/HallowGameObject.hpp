//
// Created by galex on 5/18/2021.
//

#ifndef HALLOW_HALLOWGAMEOBJECT_HPP
#define HALLOW_HALLOWGAMEOBJECT_HPP

#include <memory>
#include <engine/model/HallowModel.hpp>

namespace Hallow {
  struct Transform2d {
    glm::vec2 translation{.0f, .0f}; // (position offset)
    glm::vec2 scale{1.f, 1.f};
    float rotation;

    // glm works with array rows mapping to matrix columns, not rows!
    // think of it as each array row being a basis vector
    glm::mat2 mat2() {
      const float s = glm::sin(rotation);
      const float c = glm::cos(rotation);

      glm::mat2 rotation_mat = {
        {c, s},
        {-s, c}
      };

      glm::mat2 scale_mat {
        {scale.x, .0f},
        {.0f, scale.y}
      };

      return rotation_mat * scale_mat;
    }
  };

  class HallowGameObject {
  public:
    using id_t = unsigned int;

    static HallowGameObject createGameObject() {
      return HallowGameObject{currentGameObjectId()};
    }

    HallowGameObject(const HallowGameObject&) = delete;
    HallowGameObject& operator=(const HallowGameObject&) = delete;
    HallowGameObject(HallowGameObject&&) = default;
    HallowGameObject& operator=(HallowGameObject&&) = default;

    id_t id() const { return m_id; }
    Transform2d& transform() { return m_transform; }
    void set_transform(Transform2d transform) { m_transform = transform; }
    std::shared_ptr<HallowModel>& model() { return m_model; }
    void set_model(std::shared_ptr<HallowModel> model) { m_model = model; }
    glm::vec3& color() { return m_color; }
    void set_color(glm::vec3 color) { m_color = color; }

  protected:
    id_t m_id;

    // Components
    Transform2d m_transform{};
    std::shared_ptr<HallowModel> m_model{};
    glm::vec3 m_color{};

    HallowGameObject(id_t object_id) : m_id(object_id) {}

    static id_t currentGameObjectId() {
      static id_t current_id = 0;
      return current_id++;
    }
  };
}


#endif //HALLOW_HALLOWGAMEOBJECT_HPP
