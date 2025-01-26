#pragma once
#include "Keyboard.hpp"
#include "PerfData.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class FpsCamera {
  private:
    float m_yaw, m_pitch;
    float m_lastX, m_lastY;
    float m_sensitivity;
    float m_movementSpeed;
    float m_fov;

    glm::vec3 m_cameraUp;
    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraPos;
    bool m_isCursorVisible;

  public:
    FpsCamera(float fov, float sensitivity, float movementSpeed);
    bool m_firstMouse;
    void ProcessKeyboardInput();
    void ProcessMouseInput(float definedXPos, float definedYPos);
    void ProcessSrollInput(float definedYOffset);

    [[nodiscard]] glm::mat4 GetViewMatrix() const;
    [[nodiscard]] glm::mat4 GetProjectionMatrix(glm::vec2 viewportSize) const;
    [[nodiscard]] glm::vec3 GetCameraPosition() const;

    void SetCameraMovementSpeed(float movementSpeed);
    void SetCameraFov(float fov);
    void SetCameraSensitivity(float sensitivity);
    void SetCameraPosition(glm::vec3 position);
    void SetCameraMouseVisibility(bool isVisible);

    [[nodiscard]] float GetFov() const;
    [[nodiscard]] float GetSensitivity() const;
    [[nodiscard]] float GetMovementSpeed() const;
    [[nodiscard]] bool GetIsCursorVisible() const;
};
