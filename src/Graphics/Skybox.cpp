#include "Skybox.hpp"
#include "../Core/CameraSystem.hpp"
#include "../AssetManager.hpp"

Skybox::Skybox(const std::vector<std::string>& cubemapTextures) {
    m_cubemapTextures = cubemapTextures;
    m_textureID = Skybox::LoadTextureFromFile(cubemapTextures);
    Skybox::CreateCube();
    Skybox::CreateShader();
}

Skybox::Skybox(const std::string& folderDirectoryPath, const std::string& fileFormat) {
    std::filesystem::path folderPath(folderDirectoryPath);
    std::vector<std::string> cubemapsTextures { (folderPath / ("right" + fileFormat)).string(),
                                                (folderPath / ("left" + fileFormat)).string(),
                                                (folderPath / ("bottom" + fileFormat)).string(),
                                                (folderPath / ("top" + fileFormat)).string(),
                                                (folderPath / ("front" + fileFormat)).string(),
                                                (folderPath / ("back" + fileFormat)).string() };

    m_cubemapTextures = cubemapsTextures;
    m_textureID = Skybox::LoadTextureFromFile(cubemapsTextures);
    Skybox::CreateCube();
    Skybox::CreateShader();
}

Skybox::~Skybox() {
    delete m_cubemapShader;
}

void Skybox::RenderSkybox() {
    glm::mat4 view = glm::mat4(glm::mat3(CameraSystem::GetInstance().GetViewMatrix()));
    glm::mat4 projection = glm::perspective(
        glm::radians(CameraSystem::GetInstance().GetFov()), 16.f / 9.f, 0.1f, 150.0f);

    glDepthFunc(GL_LEQUAL);
    m_cubemapShader->SetUniform("view", view);
    m_cubemapShader->SetUniform("projection", projection);
    m_cubemapShader->Bind();
    glBindVertexArray(m_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
    OpenglAPI::DrawArrays(GL_TRIANGLES, 36);
    glDepthFunc(GL_LESS);
    glBindVertexArray(0);
}

uint32_t Skybox::LoadTextureFromFile(std::vector<std::string> cubemapTextures) {
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

    int width, height, colorChannels;
    unsigned char* data = nullptr;
    stbi_set_flip_vertically_on_load(true);

    for (unsigned int i = 0; i < cubemapTextures.size(); i++) {

        data = stbi_load(cubemapTextures[i].c_str(), &width, &height, &colorChannels, 0);
        if (data) {
            GLenum format = 3;
            if (colorChannels == 1)
                format = GL_RED;
            else if (colorChannels == 3)
                format = GL_RGB;
            else if (colorChannels == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,
                         format,
                         width,
                         height,
                         0,
                         format,
                         GL_UNSIGNED_BYTE,
                         data);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // x osa
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // y osa
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // z osa

            Log::Info("Cubemap Textura: " + cubemapTextures[i] + " uspesne nactena!");
            stbi_image_free(data);
        }
        else {
            Log::Error("Cubemap Textura : " + cubemapTextures[i] + " nebyla nactena!");
            stbi_image_free(data);
        }
    }
    return m_textureID;
}

void Skybox::CreateCube() {
    float vertices[108] = { // cubemap vertices positions
                            -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                            1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

                            -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
                            -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                            1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                            1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

                            -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                            1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

                            -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
                            1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

                            -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
                            1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

uint32_t Skybox::GetVAO() const {
    return m_vao;
}

uint32_t Skybox::GetCubeMapTex() const {
    return m_textureID;
}

void Skybox::CreateShader() {
    m_cubemapShader
        = new Shader(AssetManager::GetAssetPath(Assets::CUBEMAP_SHADER_FRAGMENT).string().c_str(),
                     AssetManager::GetAssetPath(Assets::CUBEMAP_SHADER_VERTEX).string().c_str());
}

Shader& Skybox::GetSkyboxShader() const {
    return *m_cubemapShader;
}
