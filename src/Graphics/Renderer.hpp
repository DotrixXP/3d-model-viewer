#pragma once
#include "../Core/CameraSystem.hpp"
#include "../Model/Model.hpp"
#include "ShaderManager.hpp"
#include "Skybox.hpp"

enum class PolygonMode : uint8_t {
    FILL,
    LINE,
    POINT
};

class Renderer {
  public:
    void RenderModel(Model& model, const InputData& inputData);
    void RenderAdditional();
    static Renderer& GetInstance();
    [[nodiscard]] uint32_t GetVerticesCount() const;
    [[nodiscard]] uint32_t GetIndicesCount() const;
    void ReloadShaders();
    Shader& GetShader();
    void SetActiveShader(ShaderType active);
    void SetLightIntensity(float intensity);
    void SetPolygonMode(PolygonMode mode);

  private:
    ShaderType m_activeShader;

    uint32_t m_verticesCount;
    uint32_t m_indicesCount;
    float m_lightIntensity;
    PolygonMode m_polygonMode;
    Skybox m_skybox;
    ShaderBatch m_shaderBatch;
    Renderer();
    ~Renderer() = default;
};

