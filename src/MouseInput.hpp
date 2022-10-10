#pragma once

#include "EarthGL.hpp"

class MouseInput {
private:
    static glm::vec2 m_MousePosition;
    static glm::vec2 m_ScrollOffsets;

    static void SetCursorPositionCallback(GLFWwindow *window, double x, double y);

    static void SetScrollCallback(GLFWwindow *window, double x, double y);

public:
    static void Init(GLFWwindow *window) {
        glfwSetCursorPosCallback(window, SetCursorPositionCallback);
        glfwSetScrollCallback(window, SetScrollCallback);
    }

    static void Reset() {
        m_ScrollOffsets = glm::vec2(0, 0);
    }

    static const glm::vec2 &GetMousePosition();

    static const glm::vec2 &GetScrollOffsets();
};