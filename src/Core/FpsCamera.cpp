#include "FpsCamera.hpp"

FpsCamera::FpsCamera(float fov, float sensitivity, float movementSpeed)
{
    m_fov = fov;
    m_yaw = -90.f;
    m_pitch = 0.f;
    m_lastX = 1280.f / 2.f;
    m_lastY = 720.f / 2.f;
    m_firstMouse = true;
    m_movementSpeed = movementSpeed;
    m_sensitivity = sensitivity;

    m_isCursorVisible = true;
    m_cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);    // Kde se kamera nachází
    m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // Kam se kamera kouká
    m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Vektor kamery ukazující nahoru
}

bool FpsCamera::GetIsCursorVisible() const { return m_isCursorVisible; }

void FpsCamera::ProcessKeyboardInput()
{
    float speed = m_movementSpeed * PerfData::GetDeltaTime();
    if (Keyboard::IsKeyPressed(GLFW_KEY_W))
        m_cameraPos += speed * m_cameraFront;
    if (Keyboard::IsKeyPressed(GLFW_KEY_S))
        m_cameraPos -= speed * m_cameraFront;
    if (Keyboard::IsKeyPressed(GLFW_KEY_A))
        m_cameraPos -=
            glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * speed;
    if (Keyboard::IsKeyPressed(GLFW_KEY_D))
        m_cameraPos +=
            glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * speed;
    if (Keyboard::IsKeyPressed(GLFW_KEY_SPACE))
        m_cameraPos -= speed * m_cameraUp;
    if (Keyboard::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
        m_cameraPos += speed * m_cameraUp;

    if (Keyboard::IsKeyPressed(GLFW_KEY_F1))
        m_isCursorVisible = false;
    if (Keyboard::IsKeyPressed(GLFW_KEY_F2))
        m_isCursorVisible = true;
}

void FpsCamera::ProcessMouseInput(float definedXPos, float definedYPos)
{
    if (m_firstMouse)
    {
        m_lastX = definedXPos;
        m_lastY = definedYPos;
        m_firstMouse = false;
    }

    float xoffset = definedXPos - m_lastX;
    float yoffset = definedYPos - m_lastY;
    m_lastX = definedXPos;
    m_lastY = definedYPos;

    m_yaw += xoffset * m_sensitivity;
    m_pitch += yoffset * m_sensitivity;

    // Zajistit, aby byl pitch v rozmezí, jinak by se otočil
    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_cameraFront = glm::normalize(front);
}

void FpsCamera::ProcessSrollInput(float definedXOffset, float definedYOffset)
{
    m_fov -= definedYOffset;
    if (m_fov < 1.0f)
        m_fov = 1.0f;
    if (m_fov > 65.0f)
        m_fov = 65.0f;
}

glm::mat4 FpsCamera::GetViewMatrix() const
{
    return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

glm::mat4 FpsCamera::GetProjectionMatrix(glm::vec2 viewportSize) const
{
    return glm::perspective(glm::radians(m_fov),
                            viewportSize.x / viewportSize.y, 0.1f, 260.0f);
}

glm::vec3 FpsCamera::GetCameraPosition() const { return m_cameraPos; }

void FpsCamera::SetCameraMovementSpeed(float movementSpeed)
{
    m_movementSpeed = movementSpeed;
}

void FpsCamera::SetCameraFov(float fov) { m_fov = fov; }

void FpsCamera::SetCameraSensitivity(float sensitivity)
{
    m_sensitivity = sensitivity;
}

void FpsCamera::SetCameraPosition(glm::vec3 position)
{
    m_cameraPos = position;
}

float FpsCamera::GetFov() const { return m_fov; }

float FpsCamera::GetSensitivity() const { return m_sensitivity; }

float FpsCamera::GetMovementSpeed() const { return m_movementSpeed; }
