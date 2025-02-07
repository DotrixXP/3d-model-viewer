#include "Locale.hpp"
#include "LocaleKeys.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>
#include <string>
#include "../AssetManager.hpp"
#include "../Tools/Log.hpp"
#include "../AppSettings.hpp"

std::optional<Language> Locale::s_currentLanguage = std::nullopt;
std::vector<std::string> Locale::s_localeKeys = {};

Language LanguageFromString(const std::string& language) {
    if (language == "en")
        return Language::ENGLISH;
    if (language == "cs")
        return Language::CZECH;

    Log::Error("Unsupported language: " + language, "Fallbacking to Czech");

    return Language::CZECH;
}

const std::string LanguageToString(Language language) {
    switch (language) {
    case Language::ENGLISH:
        return "en";
    case Language::CZECH:
        return "cs";
    default:
        Log::Error("Unsupported language: " + std::to_string(static_cast<uint32_t>(language)),
                   "Fallbacking to Czech");
        return "cs";
    }
}

void Locale::SetLanguage(Language language) {
    if (s_currentLanguage != language) {
        s_currentLanguage = language;
        s_localeKeys.clear();

        switch (s_currentLanguage.value()) {
        case Language::ENGLISH:
            LoadLocale(AssetManager::GetAssetPath(Assets::ENGLISH_LOCALE).string());
            break;
        case Language::CZECH:
            LoadLocale(AssetManager::GetAssetPath(Assets::CZECH_LOCALE).string());
            break;
        }
    }

    SettingsManager::Get().SetLanguage(s_currentLanguage.value());
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
        auto savedLocale = SettingsManager::Get().GetLoadedSettings().language;
        s_currentLanguage = LanguageFromString(savedLocale);
        switch (s_currentLanguage.value()) {
        case Language::ENGLISH:
            LoadLocale(AssetManager::GetAssetPath(Assets::ENGLISH_LOCALE).string());
            break;
        case Language::CZECH:
            LoadLocale(AssetManager::GetAssetPath(Assets::CZECH_LOCALE).string());
            break;
        }
    }
    return s_localeKeys[static_cast<uint32_t>(key)].c_str();
}
