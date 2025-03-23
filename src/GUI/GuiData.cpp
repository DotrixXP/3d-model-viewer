#include "GuiData.hpp"
#include "../Graphics/Renderer.hpp"
#include <glm/ext/matrix_transform.hpp>

InputData::InputData(glm::vec3 modelPos,
                     glm::vec3 modelRot,
                     glm::vec3 modelScale,
                     float modelRotAngle,
                     bool allowCameraInput)
    : m_modelPos(modelPos)
    , m_modelRot(modelRot)
    , m_modelScale(modelScale)
    , m_modelRotAngle(modelRotAngle)
    , m_allowCameraInput(allowCameraInput)
    , m_lightShaderActive(true)
    , m_simpleShaderActive(false)
    , m_reflectShaderActive(false)
    , m_lightIntensity(0.5f)
    , m_polygonMode(PolygonMode::FILL)
    , m_materialShininess(32.0f)
    , m_lightPos(1.2f, 1.0f, 2.0f)
    , m_skyboxActive(true) {
    model = glm::mat4(1.0f);
    model = glm::translate(model, modelPos);
    model = glm::rotate(model, glm::radians(modelRotAngle), modelRot);
    model = glm::scale(model, modelScale);
}

PolygonMode InputData::GetPolygonMode() const {
    return m_polygonMode;
}

bool InputData::GetSkyboxActive() const {
    return m_skyboxActive;
}

bool InputData::GetAllowCameraInput() const {
    return m_allowCameraInput;
}

ShaderType InputData::GetActiveShader() const {
    if (m_lightShaderActive)
        return ShaderType::LIGHT;
    else if (m_simpleShaderActive)
        return ShaderType::SIMPLE;
    else
        return ShaderType::REFLECT;
}

float InputData::GetLightIntensity() const {
    return m_lightIntensity;
}

glm::vec3 InputData::GetLightPos() const {
    return m_lightPos;
}

float InputData::GetMaterialShininess() const {
    return m_materialShininess;
}

const glm::mat4 InputData::GetModelMatrix() const {
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));

    return scaleMatrix * model;
}

CameraSettings::CameraSettings() noexcept
    : cameraType(0)
    , projectionType(2)
    , zoomMultiplier(1.f) {}
