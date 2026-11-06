#pragma once

#include "lux/Core.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace lux {

class Camera {
public:
    Camera() = default;

    // 2D orthographic camera
    static Camera orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near = -1.0f, f32 far = 1.0f);
    // 3D perspective camera
    static Camera perspective(f32 fov, f32 aspect, f32 near = 0.1f, f32 far = 1000.0f);

    void setPosition(const glm::vec3& pos) { m_position = pos; m_dirty = true; }
    void setRotation(const glm::vec3& rot) { m_rotation = rot; m_dirty = true; }
    void setTarget(const glm::vec3& target) { m_target = target; m_mode = Mode::LookAt; m_dirty = true; }

    void move(const glm::vec3& delta) { m_position += delta; m_dirty = true; }
    void rotate(f32 yaw, f32 pitch);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix() const { return m_projection; }
    glm::mat4 getViewProjection() { return getProjectionMatrix() * getViewMatrix(); }

    glm::vec3 getPosition() const { return m_position; }
    glm::vec3 getForward() const;
    glm::vec3 getRight() const;
    glm::vec3 getUp() const;

    void setViewport(i32 width, i32 height);

private:
    enum class Mode { Free, LookAt };
    enum class Type { Orthographic, Perspective };

    Type m_type = Type::Perspective;
    Mode m_mode = Mode::Free;

    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f); // pitch, yaw, roll
    glm::vec3 m_target = glm::vec3(0.0f);

    glm::mat4 m_projection = glm::mat4(1.0f);
    glm::mat4 m_view = glm::mat4(1.0f);
    bool m_dirty = true;

    f32 m_fov = 45.0f;
    f32 m_aspect = 16.0f / 9.0f;
    f32 m_near = 0.1f;
    f32 m_far = 1000.0f;

    // 2D params
    f32 m_left = -1, m_right = 1, m_bottom = -1, m_top = 1;

    void updateView();
};

} // namespace lux