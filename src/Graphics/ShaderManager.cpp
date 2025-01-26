#include "ShaderManager.hpp"
#include "../AssetManager.hpp"

ShaderBatch::ShaderBatch()
    : reflect(AssetManager::GetAssetPath(Assets::REFLECT_SHADER_FRAGMENT).string().c_str(),
              AssetManager::GetAssetPath(Assets::REFLECT_SHADER_VERTEX).string().c_str())
    , lightShader(AssetManager::GetAssetPath(Assets::LIGHT_SHADER_FRAGMENT).string().c_str(),
          AssetManager::GetAssetPath(Assets::LIGHT_SHADER_VERTEX).string().c_str())
    , simpleShader(AssetManager::GetAssetPath(Assets::SIMPLE_SHADER_FRAGMENT).string().c_str(),
                   AssetManager::GetAssetPath(Assets::SIMPLE_SHADER_VERTEX).string().c_str()) {}
