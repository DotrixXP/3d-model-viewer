#include "CameraSystem.hpp"

CameraSystem::CameraSystem() noexcept
    : m_activeCamera(Cameras::STATIC), m_fpsCamera(70.f, 0.1f, 10.f),
      m_orbitalCamera(), m_inputState(false) {}

CameraSystem &CameraSystem::GetInstance() {
  static CameraSystem instance;
  return instance;
}

void CameraSystem::SetFov(const float fov, const glm::vec2 viewportSize) {
  if (m_activeCamera == Cameras::STATIC) {
    m_orbitalCamera.SetFov(fov, viewportSize);
  }
}

void CameraSystem::SetProjMatToOrtho() {
  if (m_activeCamera == Cameras::STATIC) {
    m_orbitalCamera.SetProjMatToOrtho();
  }
}

void CameraSystem::SetProjMatToPerspective(const glm::vec2 viewportSize) {
  if (m_activeCamera == Cameras::STATIC) {
    m_orbitalCamera.SetProjMatToPerspective(viewportSize);
  }
}

const glm::mat4 CameraSystem::GetOrthoProjectionMatrix() {
  return glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, m_orbitalCamera.GetNear(),
                    m_orbitalCamera.GetFar());
}

void CameraSystem::SetActiveCamera(Cameras camera) { m_activeCamera = camera; }

Cameras CameraSystem::GetActiveCameraType() { return m_activeCamera; }

void CameraSystem::ProcessMouseScrollInput(const float definedXOffset,
                                           const float definedYOffset) {
  if (!m_inputState) {
    return;
  }

  if (m_activeCamera == Cameras::FIRST_PERSON) {
    m_fpsCamera.ProcessSrollInput(definedXOffset, definedYOffset);
  } else {
    m_orbitalCamera.Zoom(definedYOffset);
  }
}

void CameraSystem::ProcessMouseInput(const float definedXPos,
                                     const float definedYPos) {

  if (!m_inputState) {
    return;
  }

  if (m_activeCamera == Cameras::FIRST_PERSON) {
    m_fpsCamera.ProcessMouseInput(definedXPos, definedYPos);
  } else {
    m_orbitalCamera.ProcessMouseInput(definedXPos, definedYPos);
  }
}

void CameraSystem::UpdateInput() {
  Window::SetCursorVisible(m_fpsCamera.GetIsCursorVisible());
  
  if (!m_inputState) {
    return;
  }

  if (m_activeCamera == Cameras::FIRST_PERSON) {
    m_fpsCamera.ProcessKeyboardInput();
  }
}

void CameraSystem::SetInputState(bool state) { m_inputState = state; }

const glm::mat4 CameraSystem::GetViewMatrix() const {
  if (m_activeCamera == Cameras::FIRST_PERSON) {
    return m_fpsCamera.GetViewMatrix();
  } else {
    return m_orbitalCamera.GetViewMatrix();
  }
}

const glm::mat4 CameraSystem::GetProjectionMatrix() {
  auto viewportSize = WindowSystem::GetViewportWinSize();
  switch (m_activeCamera) {
  case Cameras::FIRST_PERSON:
    return m_fpsCamera.GetProjectionMatrix(viewportSize);
  case Cameras::STATIC:
    return m_orbitalCamera.GetProjecionMatrix(viewportSize);
  }
}
