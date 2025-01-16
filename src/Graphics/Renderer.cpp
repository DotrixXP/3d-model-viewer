#include "Renderer.hpp"
#include "../AssetManager.hpp"
#include "../Core/GLDebug.hpp"

Renderer& Renderer::GetInstance() {
    static Renderer instance;
    return instance;
}

void Renderer::RenderModel(Model& model, const InputData& inputData) {

    auto modelMatrix = inputData.GetModelMatrix();

    m_indicesCount = model.GetIndicesCount();
    m_verticesCount = model.GetVerticesCount();

    if (m_polygonMode == PolygonMode::FILL) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if (m_polygonMode == PolygonMode::LINE) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }

    if (m_activeShader == ShaderType::LIGHT) {
        m_shaderBatch.lightShader.Bind();
        m_shaderBatch.lightShader.SetUniform("projection",
                                             CameraSystem::GetInstance().GetProjectionMatrix());
        m_shaderBatch.lightShader.SetUniform("view", CameraSystem::GetInstance().GetViewMatrix());
        m_shaderBatch.lightShader.SetUniform("model", modelMatrix);
        m_shaderBatch.lightShader.SetUniform("intensityMultiplier", m_lightIntensity);
        m_shaderBatch.lightShader.SetUniform("lightPos", inputData.GetLightPos());
        m_shaderBatch.lightShader.SetUniform("shininess", inputData.GetMaterialShininess());

        m_shaderBatch.lightShader.SetMaterialUniform(model.GetMaterials());
    }
    else if (m_activeShader == ShaderType::SIMPLE) {
        m_shaderBatch.simpleShader.Bind();
        m_shaderBatch.simpleShader.SetUniform("projection",
                                              CameraSystem::GetInstance().GetProjectionMatrix());
        m_shaderBatch.simpleShader.SetUniform("view", CameraSystem::GetInstance().GetViewMatrix());
        m_shaderBatch.simpleShader.SetUniform("model", modelMatrix);
        m_shaderBatch.simpleShader.SetUniform("diffuse", 0);

        auto diffuse = model.GetMaterials().TryGetDiffuseTexture();
        auto color = model.GetMaterials().TryGetDiffuseColor();

        if (diffuse.has_value()) {
            diffuse.value().Bind(0);
            m_shaderBatch.simpleShader.SetUniform("diffuseColor", glm::vec4(0.f));
        }
        else if (color.has_value()) {
            m_shaderBatch.simpleShader.SetUniform("diffuseColor", glm::vec4(color.value(), 1.0f));
        }
    }
    else {
        m_shaderBatch.reflect.Bind();
        m_shaderBatch.reflect.SetUniform("projection",
                                         CameraSystem::GetInstance().GetProjectionMatrix());
        m_shaderBatch.reflect.SetUniform("view", CameraSystem::GetInstance().GetViewMatrix());
        m_shaderBatch.reflect.SetUniform("model", modelMatrix);
        m_shaderBatch.reflect.SetUniform("cameraPos",
                                         CameraSystem::GetInstance().GetActiveCameraType()
                                                 == Cameras::ARCBALL
                                             ? CameraSystem::GetInstance().GetViewMatrix()[3]
                                             : glm::vec3(0.0f));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox.GetCubeMapTex());
        m_shaderBatch.reflect.SetUniform("skybox", 0);
    }

    model.DrawArrays();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::RenderAdditional() {
    m_skybox.RenderSkybox();
}

uint32_t Renderer::GetVerticesCount() const {
    return m_verticesCount;
}

uint32_t Renderer::GetIndicesCount() const {
    return m_indicesCount;
}

void Renderer::SetActiveShader(ShaderType active) {
    m_activeShader = active;
}

void Renderer::SetLightIntensity(float intensity) {
    m_lightIntensity = intensity;
}

void Renderer::SetPolygonMode(PolygonMode mode) {
    if (mode == PolygonMode::FILL) {
        m_polygonMode = mode;
    }
    else if (mode == PolygonMode::LINE) {

        m_polygonMode = mode;
    }
    else {

        m_polygonMode = mode;
    }
}

Renderer::Renderer()
    : m_activeShader(ShaderType::LIGHT)
    , m_verticesCount(0)
    , m_indicesCount(0)
    , m_lightIntensity(1.0f)
    , m_polygonMode(PolygonMode::FILL)
    , m_skybox(AssetManager::GetAssetPath(Assets::SKYBOX_FOLDER), ".jpg") {}

Shader& Renderer::GetShader() {
    return m_shaderBatch.lightShader;
}
