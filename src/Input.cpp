#include "lux/Input.h"

namespace lux {

Window* Input::s_window = nullptr;
std::unordered_map<i32, bool> Input::s_keyState;
std::unordered_map<i32, bool> Input::s_keyPrevState;
std::unordered_map<i32, bool> Input::s_mouseState;
std::unordered_map<i32, bool> Input::s_mousePrevState;
glm::vec2 Input::s_mousePos = glm::vec2(0.0f);
glm::vec2 Input::s_mousePrevPos = glm::vec2(0.0f);
glm::vec2 Input::s_mouseDelta = glm::vec2(0.0f);
f32 Input::s_scrollDelta = 0.0f;
bool Input::s_firstMouse = true;

void Input::init(Window* window) {
    s_window = window;
    window->setKeyCallback(keyCallback);
    window->setMouseCallback(mouseCallback);
    window->setMouseButtonCallback(mouseButtonCallback);

    // Scroll callback via GLFW directly
    glfwSetScrollCallback(window->getNativeHandle(), [](GLFWwindow*, f64, f64 yoff) {
        s_scrollDelta += (f32)yoff;
    });

    LUX_INFO("Input system initialized");
}

void Input::update() {
    // Update previous states
    s_keyPrevState = s_keyState;
    s_mousePrevState = s_mouseState;
    s_mousePrevPos = s_mousePos;
    s_mouseDelta = s_mousePos - s_mousePrevPos;
    s_scrollDelta = 0.0f;
    s_firstMouse = false;
}

bool Input::isKeyPressed(i32 key) {
    auto it = s_keyState.find(key);
    return it != s_keyState.end() && it->second;
}

bool Input::isKeyJustPressed(i32 key) {
    bool current = isKeyPressed(key);
    auto it = s_keyPrevState.find(key);
    bool prev = it != s_keyPrevState.end() && it->second;
    return current && !prev;
}

bool Input::isKeyJustReleased(i32 key) {
    bool current = isKeyPressed(key);
    auto it = s_keyPrevState.find(key);
    bool prev = it != s_keyPrevState.end() && it->second;
    return !current && prev;
}

bool Input::isMouseButtonPressed(i32 button) {
    auto it = s_mouseState.find(button);
    return it != s_mouseState.end() && it->second;
}

bool Input::isMouseButtonJustPressed(i32 button) {
    bool current = isMouseButtonPressed(button);
    auto it = s_mousePrevState.find(button);
    bool prev = it != s_mousePrevState.end() && it->second;
    return current && !prev;
}

bool Input::isMouseButtonJustReleased(i32 button) {
    bool current = isMouseButtonPressed(button);
    auto it = s_mousePrevState.find(button);
    bool prev = it != s_mousePrevState.end() && it->second;
    return !current && prev;
}

glm::vec2 Input::getMousePosition() {
    return s_mousePos;
}

glm::vec2 Input::getMouseDelta() {
    return s_mouseDelta;
}

f32 Input::getScrollDelta() {
    return s_scrollDelta;
}

bool Input::isGamepadConnected(i32 gamepad) {
    return glfwJoystickPresent(GLFW_JOYSTICK_1 + gamepad) == GLFW_TRUE;
}

bool Input::isGamepadButtonPressed(i32 button, i32 gamepad) {
    if (!isGamepadConnected(gamepad)) return false;
    int count;
    const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1 + gamepad, &count);
    if (button < count) return buttons[button] == GLFW_PRESS;
    return false;
}

glm::vec2 Input::getGamepadAxis(i32 axis, i32 gamepad) {
    if (!isGamepadConnected(gamepad)) return glm::vec2(0.0f);
    int count;
    const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1 + gamepad, &count);
    // Simple axis mapping: 0=X, 1=Y for left stick
    if (axis == 0 && count >= 2) return glm::vec2(axes[0], axes[1]);
    if (axis == 1 && count >= 4) return glm::vec2(axes[2], axes[3]);
    return glm::vec2(0.0f);
}

void Input::keyCallback(i32 key, i32 scancode, i32 action, i32 mods) {
    s_keyState[key] = (action != GLFW_RELEASE);
}

void Input::mouseCallback(f64 x, f64 y) {
    s_mousePos = glm::vec2((f32)x, (f32)y);
}

void Input::mouseButtonCallback(i32 button, i32 action, i32 mods) {
    s_mouseState[button] = (action != GLFW_RELEASE);
}

} // namespace lux