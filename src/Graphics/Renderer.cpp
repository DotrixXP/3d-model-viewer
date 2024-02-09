#include "Renderer.hpp"

Renderer &Renderer::GetInstance() {
  static Renderer instance;
  return instance;
}

const void Renderer::RenderModel(const Model &model,
                                 const InputData &inputData) {
  m_shader.SetUniform("projection",
                      CameraSystem::GetInstance().GetProjectionMatrix());
  m_shader.SetUniform("view", CameraSystem::GetInstance().GetViewMatrix());
  auto modelMatrix = inputData.GetModelMatrix();
  m_shader.SetUniform("model", modelMatrix);
  m_indicesCount = model.GetIndicesCount();
  m_verticesCount = model.GetVerticesCount();

  model.DrawArrays(m_shader);
}

const uint32_t Renderer::GetVerticesCount() const { return m_verticesCount; }

const uint32_t Renderer::GetIndicesCount() const { return m_indicesCount; }

Renderer::Renderer()
    : m_indicesCount(0), m_verticesCount(0),
      m_shader((std::string(GLOBAL_PATH) + "res/basic.frag").c_str(),
               (std::string(GLOBAL_PATH) + "res/basic.vert").c_str()) {
}
Renderer::~Renderer() {}
