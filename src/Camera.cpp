#include "lux/Camera.h"

namespace lux {

Camera Camera::orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
    Camera cam;
    cam.m_type = Type::Orthographic;
    cam.m_left = left;
    cam.m_right = right;
    cam.m_bottom = bottom;
    cam.m_top = top;
    cam.m_near = near;
    cam.m_far = far;
    cam.m_projection = glm::ortho(left, right, bottom, top, near, far);
    return cam;
}

Camera Camera::perspective(f32 fov, f32 aspect, f32 near, f32 far) {
    Camera cam;
    cam.m_type = Type::Perspective;
    cam.m_fov = fov;
    cam.m_aspect = aspect;
    cam.m_near = near;
    cam.m_far = far;
    cam.m_projection = glm::perspective(glm::radians(fov), aspect, near, far);
    return cam;
}

void Camera::rotate(f32 yaw, f32 pitch) {
    m_rotation.y += yaw;
    m_rotation.x += pitch;
    // Clamp pitch
    if (m_rotation.x > 89.0f) m_rotation.x = 89.0f;
    if (m_rotation.x < -89.0f) m_rotation.x = -89.0f;
    m_dirty = true;
}

glm::mat4 Camera::getViewMatrix() {
    if (m_dirty) {
        updateView();
        m_dirty = false;
    }
    return m_view;
}

glm::vec3 Camera::getForward() const {
    f32 yaw = glm::radians(m_rotation.y);
    f32 pitch = glm::radians(m_rotation.x);
    return glm::normalize(glm::vec3(
        cos(yaw) * cos(pitch),
        sin(pitch),
        sin(yaw) * cos(pitch)
    ));
}

glm::vec3 Camera::getRight() const {
    return glm::normalize(glm::cross(getForward(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 Camera::getUp() const {
    return glm::normalize(glm::cross(getRight(), getForward()));
}

void Camera::setViewport(i32 width, i32 height) {
    m_aspect = (f32)width / (f32)height;
    if (m_type == Type::Perspective) {
        m_projection = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    } else {
        // Adjust orthographic projection for new aspect
        f32 halfH = (m_top - m_bottom) / 2.0f;
        f32 halfW = halfH * m_aspect;
        m_projection = glm::ortho(-halfW, halfW, -halfH, halfH, m_near, m_far);
    }
    m_dirty = true;
}

void Camera::updateView() {
    if (m_mode == Mode::LookAt) {
        m_view = glm::lookAt(m_position, m_target, glm::vec3(0.0f, 1.0f, 0.0f));
    } else {
        glm::vec3 front = getForward();
        m_view = glm::lookAt(m_position, m_position + front, glm::vec3(0.0f, 1.0f, 0.0f));
    }
}

} // namespace lux