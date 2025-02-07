#pragma once
#include "LocaleKeys.hpp"
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

Language LanguageFromString(const std::string& language);
const std::string LanguageToString(Language language);

class Locale {
  public:
    Locale() = delete;
    static void SetLanguage(Language language);
    static const char* GetText(LocaleKey key);
  private:
    static void LoadLocale(const std::string& localePath);
    static std::vector<std::string> s_localeKeys;
    static std::optional<Language> s_currentLanguage;
};
