#pragma once
#include "../Core/CameraSystem.hpp"
#include "Model.hpp"

class Renderer
{
  public:
    const void RenderModel(const Model &model, const InputData &inputData);
    static Renderer &GetInstance();
    const uint32_t GetVerticesCount() const;
    const uint32_t GetIndicesCount() const;
    void ReloadShaders();
    Shader &GetShader();

  private:
    uint32_t m_verticesCount;
    uint32_t m_indicesCount;

    Shader m_shader;
    Renderer();
    ~Renderer() = default;
};
