#pragma once
#include "../GUI/WindowSystem.hpp"
#include "../Graphics/Model.hpp"
#include "../Graphics/Renderer.hpp"
#include <filesystem>

class ModelLoader {
private:
  std::unique_ptr<Model> m_model;
  uint32_t m_texture;

public:
  ModelLoader();
  ~ModelLoader() = default;
  void LoadSelectedModel();
  void RenderSelectedModel(InputData inputData);
};
