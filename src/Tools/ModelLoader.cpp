#include "ModelLoader.hpp"

ModelLoader::ModelLoader()
{
    m_model = nullptr;
    m_texturePaths = std::vector<std::string>();
    m_texture = 0;
    m_texturePath = "";
    m_modelPath = "";
}

void ModelLoader::LoadSelectedModel()
{
    if (WindowSystem::s_modelPath.has_value())
    {
        if(m_modelPath == WindowSystem::s_modelPath.value().string())
        {
            // return;
        }

        if (m_model != nullptr)
        {
            m_texturePaths.clear();
            m_model.reset();
            m_model = nullptr;
            m_texturePaths = std::vector<std::string>();
        }

        m_model =
            std::make_unique<Model>(WindowSystem::s_modelPath.value().string());
        m_modelPath = WindowSystem::s_modelPath.value().string();

        try
        {
            m_texturePaths =
                FindTexture(WindowSystem::s_modelPath.value().string());
            m_waitingForUserInput = TEXTURE_SELECTION;

            WindowSystem::s_modelPath.reset();
        }
        catch (const std::exception &e)
        {
            Log::LogWarning("Model loading failed: " + std::string(e.what()));
            return;
        }
    }
}

void ModelLoader::RenderSelectedModel(InputData inputData)
{
    std::optional<std::string> selectedTexturePath = std::nullopt;
    if (m_waitingForUserInput == TEXTURE_SELECTION)
    {
        selectedTexturePath =
            WindowSystem::RenderTexturesDialog(m_texturePaths);
        if (selectedTexturePath.has_value())
        {
            if (m_texturePath != selectedTexturePath.value())
            {
                if (selectedTexturePath.value() != "")
                {
                    if (m_texture == 0)
                    {
                        m_texture = loadTexture(
                            selectedTexturePath.value().c_str(), GL_LINEAR);
                        m_texturePath = selectedTexturePath.value();
                    }
                    else
                    {
                        updateTexture(m_texture,
                                      selectedTexturePath.value().c_str());
                        m_texturePath = selectedTexturePath.value();
                    }

                    std::cout << "Texture loaded: " << m_texture << std::endl;
                    m_waitingForUserInput = NO_INPUT;
                }
                else
                {
                    m_waitingForUserInput = COLOR_SELECTION;
                }
            }
            else
            {
                m_waitingForUserInput = NO_INPUT;
            }
        }
    }

    if (m_waitingForUserInput == COLOR_SELECTION)
    {
        std::optional<glm::vec3> selectedColor =
            WindowSystem::RenderModelColorPicker();
        if (selectedColor.has_value())
        {
            m_model->OverwriteColor(selectedColor.value());
            m_waitingForUserInput = NO_INPUT;
        }
    }

    if (m_model != nullptr && m_waitingForUserInput == NO_INPUT)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        Renderer::GetInstance().RenderModel(*m_model, inputData);
    }
}

bool ModelLoader::IsModelLoaded() { return m_model != nullptr; }

ModelLoader::~ModelLoader()
{
    if (m_texture != 0)
    {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
}

const std::vector<std::string>
ModelLoader::FindTexture(const std::string modelPath) const
{
    std::filesystem::path path(modelPath);

    // Odstranění poslední části cesty
    path = path.parent_path();

    std::vector<std::string> textures;
    std::filesystem::recursive_directory_iterator dir(path), end;

    while (dir != end)
    {
        if (dir->is_regular_file())
        {
            std::string extension = dir->path().extension().string();
            if (extension == ".png" || extension == ".jpg" ||
                extension == ".jpeg" || extension == ".tga" ||
                extension == ".bmp")
            {
                std::cout << "Texture found: " << dir->path().string()
                          << std::endl;
                textures.push_back(dir->path().string());
            }
        }

        // Kontrola, zda 'dir' dosáhl 'end' před inkrementací
        if (++dir == end)
        {
            break;
        }
    }

    return textures;
}
