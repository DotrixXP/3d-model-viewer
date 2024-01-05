#include "StaticCamera.hpp"

StaticCamera::StaticCamera()
    : m_fov(45.0f), m_near(0.1f), m_far(500.0f), m_isOrtho(false) {
  glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
              glm::vec3(0.0f, 1.0f, 0.0f));
}

StaticCamera::~StaticCamera() {}

void StaticCamera::SetFov(const float fov, const glm::vec2 viewportSize) {
  m_fov = fov;
  m_projectionMatrix = std::nullopt;
}

void StaticCamera::SetNear(const float near, const glm::vec2 viewportSize) {
  m_near = near;
  m_projectionMatrix = std::nullopt;
}

void StaticCamera::SetFar(const float far, const glm::vec2 viewportSize) {
  m_far = far;
  m_projectionMatrix = std::nullopt;
}

void StaticCamera::SetProjMatToOrtho() {
  m_isOrtho = true;
  m_projectionMatrix = std::nullopt;
}

void StaticCamera::SetProjMatToPerspective(const glm::vec2 viewportSize) {
  m_isOrtho = false;
  m_projectionMatrix = std::nullopt;
}

void StaticCamera::RecreateProjectionMatrix(glm::vec2 viewportSize) {
  if (!m_isOrtho) {
    if (!m_projectionMatrix.has_value()) {

      m_projectionMatrix = glm::perspective(
          glm::radians(m_fov), viewportSize.x / viewportSize.y, m_near, m_far);
    }
  } else {

    if (!m_projectionMatrix.has_value()) {
      m_projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, m_near, m_far);
    }
  }
}

const glm::mat4 StaticCamera::GetProjecionMatrix(const glm::vec2 viewportSize) {
  this->RecreateProjectionMatrix(viewportSize);
  return m_projectionMatrix.value();
}

const glm::mat4 StaticCamera::GetViewMatrix() const { return m_viewMatrix; }
