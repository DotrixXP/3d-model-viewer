#pragma once
#include "Locale/LocaleKeys.hpp"
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <optional>
#include <string>

struct AppSettings {
    int fontSize;
    std::string language;

    AppSettings();

    template <class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(fontSize), CEREAL_NVP(language));
    }
};

class SettingsManager {
  public:
    // Delete copy constructor and assignment operator
    SettingsManager(const SettingsManager&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;

    // Provide a static method to get the instance
    static SettingsManager& Get();

    AppSettings& GetLoadedSettings();
    void SaveSettings(const AppSettings& settings);

    void SetLanguage(const Language language);
    void SetFontSize(const int fontSize);

  private:
    AppSettings m_settings;
    SettingsManager();
};
