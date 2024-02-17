#pragma once
#include <cstdint>
#include <string>
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif
#include <fstream>
#include <iostream>

// define NOMINMAX před windows.h -> jinak to rozbije assimp

class Log
{
  private:
    uint32_t errorCount, warningCount;

  public:
    static void LogError(const std::string &errorMesage);
    static void LogWarning(const std::string &warningMesage);
    static void LogInfo(const std::string &informationMesage);
};
