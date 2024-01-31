#include "ArcBallCamera.hpp"
#include "glm/ext/scalar_constants.hpp"

ArcBallCamera::ArcBallCamera()
    : m_fov(45.0f), m_near(0.1f), m_far(500.0f), m_isOrtho(false),
      m_firstMouse(true), m_sensitivity(0.05),
      m_center(glm::vec3(2.0f, 0.0f, -5.0f)),
      m_upVector(glm::vec3(0.0f, 1.0f, 0.0f)), m_radius(20), m_minRadius(2),
      m_azimuthAngle(0), m_polarAngle(0), m_lastXPos(0), m_lastYPos(0),
      m_rotationSpeed(0.01f) {}

ArcBallCamera::~ArcBallCamera() {}

void ArcBallCamera::SetFov(const float fov, const glm::vec2 viewportSize) {
  m_fov = fov;
  m_projectionMatrix = std::nullopt;
}

void ArcBallCamera::SetNear(const float near, const glm::vec2 viewportSize) {
  m_near = near;
  m_projectionMatrix = std::nullopt;
}

void ArcBallCamera::SetFar(const float far, const glm::vec2 viewportSize) {
  m_far = far;
  m_projectionMatrix = std::nullopt;
}

void ArcBallCamera::ProcessMouseInput(float definedXPos, float definedYPos) {
  int mouseKeyState =
      glfwGetMouseButton(Window::GetGLFWwindowRef(), GLFW_MOUSE_BUTTON_LEFT);
  if (mouseKeyState == GLFW_PRESS) {

    if (m_firstMouse) {
      m_lastXPos = definedXPos;
      m_lastYPos = definedYPos;
      m_firstMouse = false;
    }

    float deltaX = definedXPos - m_lastXPos;
    float deltaY = definedYPos - m_lastYPos;

    m_lastXPos = definedXPos;
    m_lastYPos = definedYPos;

    RotateAzimuth(deltaX * m_rotationSpeed);
    RotatePolar(deltaY * m_rotationSpeed);
  } else
    m_firstMouse = true;
}

const float ArcBallCamera::GetFov() const { return m_fov; }

const float ArcBallCamera::GetNear() const { return m_near; }

const float ArcBallCamera::GetFar() const { return m_far; }

void ArcBallCamera::RotateAzimuth(const float radians) {
  m_azimuthAngle += radians;

  const auto fullCircle = 2.0f * glm::pi<double>();
  m_azimuthAngle = fmodf(m_azimuthAngle, fullCircle);
  if (m_azimuthAngle < 0.0f) {
    m_azimuthAngle = fullCircle + m_azimuthAngle;
  }
}

void ArcBallCamera::Zoom(const float factor) {
  m_radius -= factor;
  if (m_radius < m_minRadius) {
    m_radius = m_minRadius;
  }
}

void ArcBallCamera::RotatePolar(const float radians) {
  m_polarAngle += radians;

  const auto polarCap = glm::pi<double>() / 2.0f - 0.001f;
  if (m_polarAngle > polarCap) {
    m_polarAngle = polarCap;
  }

  if (m_polarAngle < -polarCap) {
    m_polarAngle = -polarCap;
  }
}

const glm::mat4 ArcBallCamera::GetViewMatrix() const {
  const auto sineAzimuth = sin(m_azimuthAngle);
  const auto cosineAzimuth = cos(m_azimuthAngle);
  const auto sinePolar = sin(m_polarAngle);
  const auto cosinePolar = cos(m_polarAngle);

  const auto x = m_center.x + m_radius * cosinePolar * cosineAzimuth;
  const auto y = m_center.y + m_radius * sinePolar;
  const auto z = m_center.z + m_radius * cosinePolar * sineAzimuth;

  const auto cameraPos = glm::vec3(x, y, z);
  const auto normalizedView = glm::normalize(m_center - cameraPos);

  return glm::lookAt(cameraPos, cameraPos + normalizedView, m_upVector);
}

void ArcBallCamera::RecreateProjectionMatrix(const glm::vec2 viewportSize) {
  if (m_isOrtho) {
    m_projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, m_near, m_far);
  } else {
    m_projectionMatrix = glm::perspective(
        glm::radians(m_fov), viewportSize.x / viewportSize.y, m_near, m_far);
  }
}

void ArcBallCamera::SetProjMatToOrtho() {
  m_isOrtho = true;
  m_projectionMatrix = std::nullopt;
}

void ArcBallCamera::SetProjMatToPerspective(const glm::vec2 viewportSize) {
  m_isOrtho = false;
  m_projectionMatrix = std::nullopt;
}

const glm::mat4
ArcBallCamera::GetProjecionMatrix(const glm::vec2 viewportSize) {
  this->RecreateProjectionMatrix(viewportSize);
  return m_projectionMatrix.value();
}
