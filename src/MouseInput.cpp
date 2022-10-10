#include "MouseInput.hpp"

vec2 MouseInput::m_MousePosition;
vec2 MouseInput::m_ScrollOffsets;

void MouseInput::SetCursorPositionCallback(GLFWwindow *window, double x, double y) {
    m_MousePosition = vec2(static_cast<float>(x), static_cast<float>(y));
}

void MouseInput::SetScrollCallback(GLFWwindow *window, double x, double y) {
    m_ScrollOffsets = vec2(static_cast<float>(x), static_cast<float>(y));
}

const glm::vec2 &MouseInput::GetMousePosition() {
    return m_MousePosition;
}

const glm::vec2 &MouseInput::GetScrollOffsets() {
    return m_ScrollOffsets;
}