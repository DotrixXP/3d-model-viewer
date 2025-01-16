#pragma once
#include <glm/glm.hpp>
#include "../Graphics/ShaderManager.hpp"
#include "ExportWindow.hpp"

enum class PolygonMode : uint8_t;
struct InputData {
  private:
    glm::vec3 m_modelPos;
    glm::vec3 m_modelRot;
    glm::vec3 m_modelScale;
    float m_modelRotAngle;
    bool m_allowCameraInput;
    bool m_lightShaderActive;
    bool m_simpleShaderActive;
    bool m_reflectShaderActive;
    float m_lightIntensity;
    PolygonMode m_polygonMode;
    glm::mat4 model;
    float m_materialShininess;
    glm::vec3 m_lightPos;
    bool m_skyboxActive;

  public:
    InputData(glm::vec3 modelPos,
              glm::vec3 modelRot,
              glm::vec3 modelScale,
              float modelRotAngle,
              bool allowCameraInput);
    [[nodiscard]] const glm::mat4 GetModelMatrix() const;
    [[nodiscard]] bool GetAllowCameraInput() const;
    [[nodiscard]] float GetLightIntensity() const;
    [[nodiscard]] bool GetWireframeMode() const;
    [[nodiscard]] bool GetPointsMode() const;
    [[nodiscard]] PolygonMode GetPolygonMode() const;
    [[nodiscard]] glm::vec3 GetLightPos() const;
    [[nodiscard]] float GetMaterialShininess() const;
    [[nodiscard]] bool GetSkyboxActive() const;
    [[nodiscard]] ShaderType GetActiveShader() const;
    friend class WindowSystem;
};

struct CameraSettings {
    int cameraType;
    int projectionType;
    float zoomMultiplier;
    CameraSettings() noexcept;
};
