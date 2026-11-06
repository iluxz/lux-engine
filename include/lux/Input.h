#pragma once

#include "lux/Core.h"
#include "lux/Window.h"
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <glm/glm.hpp>

namespace lux {

class Input {
public:
    static void init(Window* window);
    static void update();

    // Keyboard
    static bool isKeyPressed(i32 key);
    static bool isKeyJustPressed(i32 key);
    static bool isKeyJustReleased(i32 key);

    // Mouse
    static bool isMouseButtonPressed(i32 button);
    static bool isMouseButtonJustPressed(i32 button);
    static bool isMouseButtonJustReleased(i32 button);
    static glm::vec2 getMousePosition();
    static glm::vec2 getMouseDelta();
    static f32 getScrollDelta();

    // Gamepad (basic)
    static bool isGamepadConnected(i32 gamepad = 0);
    static bool isGamepadButtonPressed(i32 button, i32 gamepad = 0);
    static glm::vec2 getGamepadAxis(i32 axis, i32 gamepad = 0);

private:
    static Window* s_window;
    static std::unordered_map<i32, bool> s_keyState;
    static std::unordered_map<i32, bool> s_keyPrevState;
    static std::unordered_map<i32, bool> s_mouseState;
    static std::unordered_map<i32, bool> s_mousePrevState;
    static glm::vec2 s_mousePos;
    static glm::vec2 s_mousePrevPos;
    static glm::vec2 s_mouseDelta;
    static f32 s_scrollDelta;
    static bool s_firstMouse;

    static void keyCallback(i32 key, i32 scancode, i32 action, i32 mods);
    static void mouseCallback(f64 x, f64 y);
    static void mouseButtonCallback(i32 button, i32 action, i32 mods);
    static void scrollCallback(f64 xoffset, f64 yoffset);
};

} // namespace lux