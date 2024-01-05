#include "CameraSystem.hpp"

CameraSystem::CameraSystem() noexcept
    : m_activeCamera(Cameras::STATIC), m_fpsCamera(70.f, 0.1f, 10.f),
      m_staticCamera() {}

CameraSystem &CameraSystem::GetInstance() {
  static CameraSystem instance;
  return instance;
}

void CameraSystem::SetFov(const float fov, const glm::vec2 viewportSize) {
  if (m_activeCamera == Cameras::STATIC) {
    m_staticCamera.SetFov(fov, viewportSize);
  }
}

void CameraSystem::SetProjMatToOrtho() {
  if (m_activeCamera == Cameras::STATIC) {
    m_staticCamera.SetProjMatToOrtho();
  }
}

void CameraSystem::SetProjMatToPerspective(const glm::vec2 viewportSize) {
  if (m_activeCamera == Cameras::STATIC) {
    m_staticCamera.SetProjMatToPerspective(viewportSize);
  }
}

void CameraSystem::SetActiveCamera(Cameras camera) { m_activeCamera = camera; }

Cameras CameraSystem::GetActiveCamera() { return m_activeCamera; }

void CameraSystem::ProcessMouseScrollInput(const float definedXOffset,
                                           const float definedYOffset) {
  if (m_activeCamera == Cameras::FIRST_PERSON) {
    m_fpsCamera.ProcessSrollInput(definedXOffset, definedYOffset);
  }
}

void CameraSystem::ProcessMouseInput(const float definedXPos,
                                     const float definedYPos) {
  if (m_activeCamera == Cameras::FIRST_PERSON) {
    m_fpsCamera.ProcessMouseInput(definedXPos, definedYPos);
  }
}

void CameraSystem::UpdateInput() {
  if (m_activeCamera == Cameras::FIRST_PERSON) {
    m_fpsCamera.ProcessKeyboardInput();
  }
  Window::SetCursorVisible(m_fpsCamera.GetIsCursorVisible());
}

glm::mat4 CameraSystem::GetViewMatrix() {
  if (m_activeCamera == Cameras::FIRST_PERSON) {
    return m_fpsCamera.GetViewMatrix();
  }
  return glm::mat4(1.0f);
}

glm::mat4 CameraSystem::GetProjectionMatrix() {
  auto viewportSize = WindowSystem::GetViewportWinSize();
  switch (m_activeCamera) {
  case Cameras::FIRST_PERSON:
    return m_fpsCamera.GetProjectionMatrix();
  case Cameras::STATIC:
    return m_staticCamera.GetProjecionMatrix(viewportSize);
  default:
    return glm::mat4(1.0f);
  }
}
