#include "ModelHandler.hpp"
#include "../Texture/Texture.hpp"

ModelHandler::ModelHandler()
    : m_modelLoader()
    , m_model(nullptr)
    , m_texturePaths(std::vector<std::string>()) {}

void ModelHandler::LoadSelectedModel() {
    if (WindowSystem::s_newModelPath.has_value()) {
        m_materialSelection = MaterialSelection();
        if (m_model != nullptr) {
            m_texturePaths.clear();
            m_model.reset();
            m_model = nullptr;
            m_texturePaths = std::vector<std::string>();
        }

        m_modelLoader.TryStartModelLoading(WindowSystem::s_newModelPath.value().string());
        auto model = m_modelLoader.RetrieveLoadedModel();

        if (model.has_value()) {
            Log::Info("Model loaded successfully");
            m_model = std::make_unique<Model>(std::move(model.value()));
        }
        else {
            Log::Error("Model loading failed");
            return;
        }
        try {
            m_texturePaths = FindTexture(WindowSystem::s_newModelPath.value().string());
            m_waitingForUserInput = USER_INPUT::TEXTURE_SELECTION;

            WindowSystem::s_newModelPath.reset();
        }
        catch (const std::exception& e) {
            Log::Warn("Model loading failed: " + std::string(e.what()));
            return;
        }
        m_waitingForUserInput = USER_INPUT::TEXTURE_SELECTION;

        WindowSystem::s_newModelPath.reset();
    }
}

const std::vector<std::string> ModelHandler::FindTexture(const std::string& modelPath) const {
    std::filesystem::path path(modelPath);

    path = path.parent_path();

    std::vector<std::string> textures;
    std::filesystem::recursive_directory_iterator dir(path), end;

    while (dir != end && textures.size() < 25) {
        if (dir->is_regular_file()) {
            std::string extension = dir->path().extension().string();
            if (std::find(SUPPORTED_TEXTURE_EXTENSIONS.begin(),
                          SUPPORTED_TEXTURE_EXTENSIONS.end(),
                          extension)
                != SUPPORTED_TEXTURE_EXTENSIONS.end()) {
                textures.push_back(dir->path().string());
            }
        }

        if (++dir == end) {
            break;
        }
    }

    return textures;
}

void ModelHandler::ExportSelectedModel(const ExportData& exportData) {
    if (m_model != nullptr) {
        m_modelExporter.ExportModel(*m_model, exportData.exportPath, exportData.exportFormat);
    }
}

void ModelHandler::RenderSelectedModel(InputData inputData) {

    if (m_waitingForUserInput == USER_INPUT::TEXTURE_SELECTION) {
        bool selected = WindowSystem::RenderMaterialDialog(m_materialSelection, m_texturePaths);

        // Check for selected textures
        if (selected) {
            if (m_materialSelection.diffuse.has_value()) {
                auto* diffuseStr = std::get_if<std::string>(&m_materialSelection.diffuse.value());
                if (diffuseStr && !diffuseStr->empty()) {
                    auto diffuseTex = m_model->GetMaterials().TryGetDiffuseTexture();
                    if (diffuseTex.has_value()) {
                        diffuseTex.value().Destroy();
                    }
                    m_model->GetMaterials().diffuse
                        = Texture(*diffuseStr, WindowSystem::s_flipTexture);
                    m_waitingForUserInput = USER_INPUT::NO_INPUT;
                }
                else {
                    auto* diffuseColor
                        = std::get_if<glm::vec3>(&m_materialSelection.diffuse.value());
                    if (diffuseColor) {

                        m_model->GetMaterials().diffuse = *diffuseColor;
                        m_waitingForUserInput = USER_INPUT::NO_INPUT;
                    }
                }

                if (m_materialSelection.specular.has_value()
                    && m_materialSelection.diffuse.has_value()) {
                    if (!m_materialSelection.specular.value().empty()) {
                        Log::Info("Specular texture: " + m_materialSelection.specular.value());
                        auto specularTex = m_model->GetMaterials().specular;

                        if (specularTex.has_value()) {
                            m_model->GetMaterials().specular.value().Destroy();
                        }

                        m_model->GetMaterials().specular = Texture(
                            m_materialSelection.specular.value(), WindowSystem::s_flipTexture);
                    }
                }
            }
        }
    }

    if (m_model != nullptr && m_waitingForUserInput == USER_INPUT::NO_INPUT) {
        m_materialSelection = MaterialSelection();
        Renderer::GetInstance().RenderModel(*m_model, inputData);
    }
}

bool ModelHandler::IsModelLoaded() {
    return m_model != nullptr;
}
