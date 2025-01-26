#include "Locale.hpp"
#include "LocaleKeys.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "../AssetManager.hpp"
#include "../Tools/Log.hpp"

Language Locale::s_currentLanguage = Language::CZECH;
std::vector<std::string> Locale::s_localeKeys = {};

void Locale::SetLanguage(Language language) {
    if (s_currentLanguage != language) {
        s_currentLanguage = language;
        s_localeKeys.clear();

        switch (s_currentLanguage) {
        case Language::ENGLISH:
            LoadLocale(AssetManager::GetAssetPath(Assets::ENGLISH_LOCALE).string());
            break;
        case Language::CZECH:
            LoadLocale(AssetManager::GetAssetPath(Assets::CZECH_LOCALE).string());
            break;
        }
    }
}

void Locale::LoadLocale(const std::string& localePath) {
    std::ifstream file(localePath);
    if (!file.is_open()) {
        Log::Error("Failed to open file: ", localePath);
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        s_localeKeys.push_back(line);
    }

    file.close();
}

const char* Locale::GetText(LocaleKey key) {
  if (s_localeKeys.empty()) {
        LoadLocale(AssetManager::GetAssetPath(Assets::CZECH_LOCALE).string().c_str());
  }
  return s_localeKeys[static_cast<uint32_t>(key)].c_str();
}
