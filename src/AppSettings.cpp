#include "AppSettings.hpp"
#include "AssetManager.hpp"
#include <cereal/archives/json.hpp>
#include <cereal/types/optional.hpp>
#include <fstream>
#include "Tools/Assert.hpp"

AppSettings::AppSettings()
    : fontSize(20)
    , language("en") {}

SettingsManager& SettingsManager::Get() {
    static SettingsManager instance;
    return instance;
}

SettingsManager::SettingsManager() {
    std::filesystem::path path = AssetManager::GetAppSettingsPath();
    if (std::filesystem::exists(path)) {
        std::ifstream is(path);
        cereal::JSONInputArchive archive(is);
        archive(m_settings);
    }
    else {
        m_settings = AppSettings();
    }
}

AppSettings& SettingsManager::GetLoadedSettings() {
    return m_settings;
}

void SettingsManager::SetLanguage(const Language language) {
    m_settings.language = language == Language::ENGLISH ? "en" : "cs";
    SaveSettings(m_settings);
}

void SettingsManager::SetFontSize(const int fontSize) {
    m_settings.fontSize = fontSize;
    SaveSettings(m_settings);
}

void SettingsManager::SaveSettings(const AppSettings& settings) {
    std::filesystem::path path = AssetManager::GetAppSettingsPath();
    std::filesystem::create_directories(path.parent_path());
    std::ofstream os(path);
    if (!os.is_open()) {
        PANIC("Failed to open settings file for writing");
    }
    cereal::JSONOutputArchive archive(os);
    archive(settings);
}
