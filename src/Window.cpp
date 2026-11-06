#include "lux/Window.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace lux {

static bool s_glfwInitialized = false;

void Window::init() {
    if (s_glfwInitialized) return;
    if (!glfwInit()) {
        LUX_FATAL("Failed to initialize GLFW");
        return;
    }
    s_glfwInitialized = true;
    LUX_INFO("GLFW initialized");
}

void Window::shutdown() {
    if (s_glfwInitialized) {
        glfwTerminate();
        s_glfwInitialized = false;
        LUX_INFO("GLFW shutdown");
    }
}

Window::Window(const WindowConfig& config)
    : m_width(config.width), m_height(config.height), m_title(config.title) {

    if (!s_glfwInitialized) init();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    if (config.fullscreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        m_window = glfwCreateWindow(mode->width, mode->height, config.title.c_str(), monitor, nullptr);
    } else {
        m_window = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);
    }

    if (!m_window) {
        LUX_FATAL("Failed to create GLFW window");
        return;
    }

    glfwMakeContextCurrent(m_window);

    // Load OpenGL
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        LUX_FATAL("Failed to initialize GLAD");
        return;
    }

    LUX_INFO("OpenGL %s", glGetString(GL_VERSION));
    LUX_INFO("Renderer: %s", glGetString(GL_RENDERER));

    glfwSwapInterval(config.vsync ? 1 : 0);

    // Set callbacks
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, onWindowResize);
    glfwSetKeyCallback(m_window, onKey);
    glfwSetCursorPosCallback(m_window, onMouseMove);
    glfwSetMouseButtonCallback(m_window, onMouseButton);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

bool Window::isKeyPressed(i32 key) const {
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

bool Window::isMouseButtonPressed(i32 button) const {
    return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
}

glm::vec2 Window::getMousePosition() const {
    f64 x, y;
    glfwGetCursorPos(m_window, &x, &y);
    return glm::vec2((f32)x, (f32)y);
}

void Window::onWindowResize(GLFWwindow* window, i32 width, i32 height) {
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (self) {
        self->m_width = width;
        self->m_height = height;
        glViewport(0, 0, width, height);
        if (self->m_resizeCallback) {
            self->m_resizeCallback(width, height);
        }
    }
}

void Window::onKey(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (self && self->m_keyCallback) {
        self->m_keyCallback(key, scancode, action, mods);
    }
}

void Window::onMouseMove(GLFWwindow* window, f64 x, f64 y) {
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (self && self->m_mouseCallback) {
        self->m_mouseCallback(x, y);
    }
}

void Window::onMouseButton(GLFWwindow* window, i32 button, i32 action, i32 mods) {
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (self && self->m_mouseButtonCallback) {
        self->m_mouseButtonCallback(button, action, mods);
    }
}

} // namespace lux