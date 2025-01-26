#pragma once
#include "LocaleKeys.hpp"
#include <cstdint>
#include <string>
#include <vector>

class Locale {
  public:
    Locale() = delete;
    static void SetLanguage(Language language);
    static const char* GetText(LocaleKey key);
  private:
    static void LoadLocale(const std::string& localePath);
    static std::vector<std::string> s_localeKeys;
    static Language s_currentLanguage;
};
