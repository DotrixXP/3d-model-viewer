#pragma once
#include "Model.hpp"
#include "../GUI/WindowSystem.hpp"
#include "../Graphics/Renderer.hpp"
#include "ModelLoader.hpp"
#include <filesystem>

enum class USER_INPUT : uint8_t {
    TEXTURE_SELECTION,
    NO_INPUT
};

class ModelHandler {
  public:
    ModelHandler();
    ~ModelHandler() = default;
    void LoadSelectedModel();
    void ExportSelectedModel(const ExportData& exportData);
    void RenderSelectedModel(InputData inputData);
    bool IsModelLoaded();

  private:
    [[nodiscard]] const std::vector<std::string> FindTexture(const std::string& modelPath) const;
    ModelLoader m_modelLoader;
    ModelExporter m_modelExporter;
    MaterialSelection m_materialSelection;
    std::unique_ptr<Model> m_model;
    USER_INPUT m_waitingForUserInput = USER_INPUT::NO_INPUT;
    std::vector<std::string> m_texturePaths;
};
