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
  this->CalculateModelMatrix(inputData);
  m_shader.SetUniform("model", m_model);
  m_indicesCount = model.GetIndicesCount();
  m_verticesCount = model.GetVerticesCount();

  model.DrawArrays(m_shader);
}

void Renderer::CalculateModelMatrix(const InputData &inputData) {
  m_model = glm::mat4(1.0f);
  m_model = glm::translate(m_model, inputData.modelPos);
  m_model = glm::rotate(m_model, glm::radians(inputData.modelRotAngle),
                        inputData.modelRot);
  m_model = glm::scale(m_model, inputData.modelScale);
}

const uint32_t Renderer::GetVerticesCount() const { return m_verticesCount; }

const uint32_t Renderer::GetIndicesCount() const { return m_indicesCount; }

Renderer::Renderer()
    : m_shader(Shader("../../src/basic.frag", "../../src/basic.vert")),
      m_model(glm::mat4(1.f)), m_indicesCount(0), m_verticesCount(0) {}

Renderer::~Renderer() {}
