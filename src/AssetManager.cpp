#include "AssetManager.hpp"
#include "Tools/Assert.hpp"

std::filesystem::path AssetManager::s_assetsFolder = "";

const std::filesystem::path AssetManager::GetAssetPath(Assets asset) {
    FindAssetFolder();

    switch (asset) {
    case Assets::REFLECT_SHADER_VERTEX:
        return s_assetsFolder / "reflect.vert.glsl";
    case Assets::LIGHT_SHADER_VERTEX:
        return s_assetsFolder / "light.vert.glsl";
    case Assets::SIMPLE_SHADER_VERTEX:
        return s_assetsFolder / "simple.vert.glsl";
    case Assets::REFLECT_SHADER_FRAGMENT:
        return s_assetsFolder / "reflect.frag.glsl";
    case Assets::LIGHT_SHADER_FRAGMENT:
        return s_assetsFolder / "light.frag.glsl";
    case Assets::SIMPLE_SHADER_FRAGMENT:
        return s_assetsFolder / "simple.frag.glsl";
    case Assets::SKYBOX_FOLDER:
        return s_assetsFolder / "skybox";
    case Assets::UI_FONT:
        return s_assetsFolder / "font.ttf";
    case Assets::WINDOW_ICON:
        return s_assetsFolder / "icon.png";
    case Assets::ENGLISH_LOCALE:
        return s_assetsFolder / "locale" / "en.loc";
    case Assets::CZECH_LOCALE:
        return s_assetsFolder / "locale" / "cs.loc";
    default:
        PANIC("Asset not found");
        return "";
    }
}

void AssetManager::FindAssetFolder() {
    if (!s_assetsFolder.empty()) {
        return;
    }

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path resFolder = "res";

    while (!std::filesystem::exists(currentPath / resFolder)) {
        if (currentPath.has_parent_path()) {
            currentPath = currentPath.parent_path();
        }
        else {
            PANIC("Asset folder not found");
        }
    }

    s_assetsFolder = currentPath / resFolder;
}
