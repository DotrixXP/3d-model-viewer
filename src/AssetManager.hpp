#pragma once

#include <filesystem>
enum class Assets : uint8_t {
    // Shaders
    REFLECT_SHADER_VERTEX,
    LIGHT_SHADER_VERTEX,
    SIMPLE_SHADER_VERTEX,
    
    REFLECT_SHADER_FRAGMENT,
    LIGHT_SHADER_FRAGMENT,
    SIMPLE_SHADER_FRAGMENT,
    
    CUBEMAP_SHADER_VERTEX,
    CUBEMAP_SHADER_FRAGMENT,

    // Cubemaps
    SKYBOX_FOLDER,

    // Fonts
    UI_FONT,

    // Icons
    WINDOW_ICON,

    // Locale
    ENGLISH_LOCALE,
    CZECH_LOCALE
};

class AssetManager {
  public:
    [[nodiscard]] static const std::filesystem::path GetAssetPath(Assets asset);
    [[nodiscard]] static const std::filesystem::path GetAppSettingsPath();
  private:
    static std::filesystem::path s_assetsFolder;
    static void FindAssetFolder();
};
