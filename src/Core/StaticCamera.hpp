#pragma once
#include "glm/glm.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <optional>

class StaticCamera {
  public:
    StaticCamera();
    ~StaticCamera();
    void SetFov(const float fov, const glm::vec2 viewportSize);
    void SetNear(const float near, const glm::vec2 viewportSize);
    void SetFar(const float far, const glm::vec2 viewportSize);
    void SetProjMatToOrtho();
    void SetProjMatToPerspective(const glm::vec2 viewportSize);
    
    const glm::mat4 GetProjecionMatrix(const glm::vec2 viewportSize);
    const glm::mat4 GetViewMatrix() const;
    const float GetFov() const;
    const float GetNear() const;
    const float GetFar() const;
  private:
    void RecreateProjectionMatrix(const glm::vec2 viewportSize);
    
    bool m_isOrtho;
    float m_fov;
    float m_near;
    float m_far;
    std::optional<glm::mat4> m_projectionMatrix;
    glm::mat4 m_viewMatrix;
};
