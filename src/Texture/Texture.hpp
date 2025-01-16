#pragma once
#include <filesystem>
class Texture {
  public:
    Texture(const std::filesystem::path& texturePath, bool flipTexture);
    ~Texture() = default;
    void Bind(uint32_t index) const;
    void Destroy();
    [[nodiscard]] uint32_t GetId() const;

  private:
    uint32_t m_textureID;
    int m_width, m_height, m_colorChannels;
    void LoadTexture(const std::filesystem::path& texturePath, bool flipTexture);
};
