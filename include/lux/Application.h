#pragma once

#include "lux/Core.h"
#include "lux/Window.h"
#include <glm/glm.hpp>
#include <memory>
#include <functional>

namespace lux {

class Application {
public:
    Application(const WindowConfig& windowConfig = WindowConfig{});
    virtual ~Application();

    void run();
    void close();

    Window& getWindow() { return *m_window; }
    f64 getDeltaTime() const { return m_deltaTime; }
    f64 getTime() const { return m_time; }

    // Virtual methods for subclasses
    virtual void onInit() {}
    virtual void onUpdate(f64 dt) {}
    virtual void onRender() {}
    virtual void onShutdown() {}

    static Application* get() { return s_instance; }

private:
    void updateTiming();

    std::unique_ptr<Window> m_window;
    f64 m_deltaTime = 0.0;
    f64 m_time = 0.0;
    f64 m_lastFrameTime = 0.0;
    bool m_running = true;

    static Application* s_instance;
};

} // namespace lux