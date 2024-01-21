#include "ModelLoader.hpp"

ModelLoader::ModelLoader() { m_model = nullptr; }

void ModelLoader::LoadSelectedModel() {
  if (WindowSystem::s_modelPath.has_value()) {
    m_model =
        std::make_unique<Model>(WindowSystem::s_modelPath.value().string());
    uint32_t texture = loadTexture("/home/dominik/Plocha/3d-model-viewer/res/backpack/diffuse.jpg", GL_LINEAR);
    m_model->OverwriteTexture(texture);

    Log::LogInfo("Model loaded: " + WindowSystem::s_modelPath.value().string());
    WindowSystem::s_modelPath.reset();
  }
}

void ModelLoader::RenderSelectedModel(InputData inputData) {
  if (m_model != nullptr) {
    Renderer::GetInstance().RenderModel(*m_model, inputData);
  }
}
