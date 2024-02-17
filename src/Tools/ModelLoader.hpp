#pragma once
#include "../GUI/WindowSystem.hpp"
#include "../Graphics/Model.hpp"
#include "../Graphics/Renderer.hpp"
#include <filesystem>

enum USER_INPUT
{
    TEXTURE_SELECTION,
    COLOR_SELECTION,
    NO_INPUT
};

class ModelLoader
{
  private:
    std::unique_ptr<Model> m_model;
    uint32_t m_texture;
    USER_INPUT m_waitingForUserInput = NO_INPUT;
    std::vector<std::string> m_texturePaths;

  public:
    ModelLoader();
    ~ModelLoader() = default;
    void LoadSelectedModel();
    void RenderSelectedModel(InputData inputData);
    bool IsModelLoaded();
    const std::vector<std::string>
    FindTexture(const std::string modelPath) const;
};
