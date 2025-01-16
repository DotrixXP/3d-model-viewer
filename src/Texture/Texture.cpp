#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "glad/glad.h"
#include "stb_image.h"
#include "../Tools/Log.hpp"

Texture::Texture(const std::filesystem::path& texturePath, bool flipTexture)
    : m_textureID(0)
    , m_width(0)
    , m_height(0)
    , m_colorChannels(0) {
    LoadTexture(texturePath, flipTexture);
}

void Texture::Bind(uint32_t index) const {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glActiveTexture(GL_TEXTURE0);
}

uint32_t Texture::GetId() const {
    return m_textureID;
}

void Texture::LoadTexture(const std::filesystem::path& texturePath, bool flipTexture) {
    glGenTextures(1, &m_textureID);

    int width, height, colorChannels;

    uint8_t* data = nullptr;
    auto filtering = GL_LINEAR;

    stbi_set_flip_vertically_on_load(flipTexture);
    data = stbi_load(texturePath.c_str(), &width, &height, &colorChannels, 0);
    if (data) {
        GLenum format = 3;
        if (colorChannels == 1)
            format = GL_RED;
        else if (colorChannels == 3)
            format = GL_RGB;
        else if (colorChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     static_cast<int32_t>(format),
                     width,
                     height,
                     0,
                     format,
                     GL_UNSIGNED_BYTE,
                     data);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (colorChannels == 4) { // Transparent textues should not repeat, because it can cause
                                  // non-transparent edges
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        if (filtering == GL_NEAREST) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else if (filtering == GL_LINEAR) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else
            Log::Error("Invalid filtering mode!");

        Log::Info("Texture: ", texturePath, " was loaded!");
        m_colorChannels = colorChannels;
        m_width = width;
        m_height = height;
        stbi_image_free(data);
    }
    else {
        Log::Error("Texture: ", texturePath, " was not loaded!");
        stbi_image_free(data);
    }
}

void Texture::Destroy() {
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
        m_width = 0;
        m_height = 0;
        m_colorChannels = 0;
    }
}
