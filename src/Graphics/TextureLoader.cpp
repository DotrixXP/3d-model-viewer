#include "TextureLoader.hpp"

uint32_t loadTexture(const std::string &directoryPath, uint32_t filtering, bool flipTexture)
{
    uint32_t m_textureID;
    glGenTextures(1, &m_textureID);

    int width, height, colorChannels;

    unsigned char *data = nullptr;

    stbi_set_flip_vertically_on_load(flipTexture);
    data = stbi_load(directoryPath.c_str(), &width, &height, &colorChannels, 0);
    if (data)
    {
        GLenum format = 3;
        if (colorChannels == 1)
            format = GL_RED;
        else if (colorChannels == 3)
            format = GL_RGB;
        else if (colorChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (colorChannels == 4)
        { // Transparentní textury se nesmí opakovat,
          // protože to může způsobit neprůhledné okraje
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        if (filtering == GL_NEAREST)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else if (filtering == GL_LINEAR)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else
            Log::LogError("Neplatny filtering!");

        Log::LogInfo("Textura: " + directoryPath + " uspesne nactena!");

        stbi_image_free(data);
    }
    else
    {
        Log::LogError("Chyba! Textura: " + directoryPath + " nebyla nactena!");
        stbi_image_free(data);
    }
    return m_textureID;
}

void updateTexture(uint32_t textureID, const std::string &directoryPath, bool flipTexture)
{
    int width, height, colorChannels;

    unsigned char *data = nullptr;

    stbi_set_flip_vertically_on_load(flipTexture);
    data = stbi_load(directoryPath.c_str(), &width, &height, &colorChannels, 0);
    if (data)
    {
        GLenum format = 3;
        if (colorChannels == 1)
            format = GL_RED;
        else if (colorChannels == 3)
            format = GL_RGB;
        else if (colorChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (colorChannels == 4)
        { // Transparentní textury se nesmí opakovat,
          // protože to může způsobit neprůhledné okraje
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Log::LogInfo("Textura: " + directoryPath + " uspesne nactena!");

        stbi_image_free(data);
    }
    else
    {
        Log::LogError("Chyba! Textura: " + directoryPath + " nebyla nactena!");
        throw std::runtime_error("Chyba! Textura: " + directoryPath +
                                 " nebyla nactena!");
        stbi_image_free(data);
    }
}
