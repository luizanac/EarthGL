#include "Camera.hpp"
#include "MouseInput.hpp"

Camera *Camera::m_Instance;

void Camera::Move(ECameraMovement direction, float deltaTime) {
    float velocity = m_MovementSpeed * deltaTime;
    if (direction == FORWARD)
        m_Position += m_Front * velocity;
    if (direction == BACKWARD)
        m_Position -= m_Front * velocity;
    if (direction == LEFT)
        m_Position -= m_Right * velocity;
    if (direction == RIGHT)
        m_Position += m_Right * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= m_MouseSensitivity;
    yOffset *= m_MouseSensitivity;

    m_Rotation.y += xOffset;
    m_Rotation.x += yOffset;

    if (constrainPitch) {
        if (m_Rotation.x > 80.0f)
            m_Rotation.x = 80.0f;
        if (m_Rotation.x < -80.0f)
            m_Rotation.x = -80.0f;
    }

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
    glm::vec3 front;
    front.x = (float) std::cos((float) glm::radians(m_Rotation.y)) * (float) cos(glm::radians(m_Rotation.x));
    front.y = (float) std::sin(glm::radians(m_Rotation.x));
    front.z = (float) std::sin((float) glm::radians(m_Rotation.y)) * (float) cos(glm::radians(m_Rotation.x));

    m_Front = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

float Camera::GetFov() const {
    return m_Fov;
}

Camera &Camera::Init(int &screenWidth, int &screenHeight, vec3 position, vec3 rotation, vec3 up) {
    if (m_Instance == nullptr)
        m_Instance = new Camera(screenWidth, screenHeight, position, rotation, up);

    return *m_Instance;
}

Camera &Camera::Get() {
    return *m_Instance;
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::perspective(
            glm::radians(GetFov()),
            (float) m_ScreenWidth / (float) m_ScreenHeight, 0.01f, 500.0f);
}

void Camera::Update(const float &deltaTime) {
    auto mousePos = MouseInput::GetMousePosition();
    float xOffset = mousePos.x - m_LastX;
    float yOffset = m_LastY - mousePos.y;

    m_LastX = mousePos.x;
    m_LastY = mousePos.y;
    Camera::Get().ProcessMouseMovement(xOffset, yOffset);
}