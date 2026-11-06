#pragma once

#include "lux/Core.h"
#include <glm/glm.hpp>
#include <functional>

// Forward declare GLFWwindow
struct GLFWwindow;

namespace lux {

struct WindowConfig {
    std::string title = "Lux Engine";
    i32 width = 1280;
    i32 height = 720;
    bool vsync = true;
    bool resizable = true;
    bool fullscreen = false;
};

class Window {
public:
    Window(const WindowConfig& config);
    ~Window();

    bool shouldClose() const;
    void pollEvents();
    void swapBuffers();

    i32 getWidth() const { return m_width; }
    i32 getHeight() const { return m_height; }
    f32 getAspectRatio() const { return (f32)m_width / (f32)m_height; }
    GLFWwindow* getNativeHandle() const { return m_window; }

    // Callbacks
    using ResizeCallback = std::function<void(i32, i32)>;
    using KeyCallback = std::function<void(i32 key, i32 scancode, i32 action, i32 mods)>;
    using MouseCallback = std::function<void(f64 x, f64 y)>;
    using MouseButtonCallback = std::function<void(i32 button, i32 action, i32 mods)>;

    void setResizeCallback(ResizeCallback cb) { m_resizeCallback = cb; }
    void setKeyCallback(KeyCallback cb) { m_keyCallback = cb; }
    void setMouseCallback(MouseCallback cb) { m_mouseCallback = cb; }
    void setMouseButtonCallback(MouseButtonCallback cb) { m_mouseButtonCallback = cb; }

    // Input queries
    bool isKeyPressed(i32 key) const;
    bool isMouseButtonPressed(i32 button) const;
    glm::vec2 getMousePosition() const;

    static void init();
    static void shutdown();

private:
    GLFWwindow* m_window = nullptr;
    i32 m_width = 0;
    i32 m_height = 0;
    std::string m_title;

    ResizeCallback m_resizeCallback;
    KeyCallback m_keyCallback;
    MouseCallback m_mouseCallback;
    MouseButtonCallback m_mouseButtonCallback;

    static void onWindowResize(GLFWwindow* window, i32 width, i32 height);
    static void onKey(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);
    static void onMouseMove(GLFWwindow* window, f64 x, f64 y);
    static void onMouseButton(GLFWwindow* window, i32 button, i32 action, i32 mods);
};

} // namespace lux