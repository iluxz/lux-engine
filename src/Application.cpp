#include "lux/Application.h"
#include <GLFW/glfw3.h>

namespace lux {

Application* Application::s_instance = nullptr;

Application::Application(const WindowConfig& windowConfig) {
    s_instance = this;
    Window::init();
    m_window = std::make_unique<Window>(windowConfig);
    LUX_INFO("Application initialized");
}

Application::~Application() {
    onShutdown();
    m_window.reset();
    Window::shutdown();
    s_instance = nullptr;
    LUX_INFO("Application shutdown");
}

void Application::run() {
    onInit();
    m_lastFrameTime = glfwGetTime();

    while (m_running && !m_window->shouldClose()) {
        updateTiming();

        m_window->pollEvents();
        onUpdate(m_deltaTime);
        onRender();
        m_window->swapBuffers();
    }
}

void Application::close() {
    m_running = false;
}

void Application::updateTiming() {
    f64 currentTime = glfwGetTime();
    m_deltaTime = currentTime - m_lastFrameTime;
    m_lastFrameTime = currentTime;
    m_time = currentTime;
}

} // namespace lux