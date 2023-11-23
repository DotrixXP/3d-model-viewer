#pragma once
#include <cstdint>
#include <string>
// #include <windows.h>
#include <fstream>
#include <iostream>

// define NOMINMAX p�ed windows.h -> jinak to rozbije assimp a mo�n� dal��

class Log {
private:
  uint32_t errorCount, warningCount;

public:
  static void LogError(const std::string &errorMesage);
  static void LogWarning(const std::string &warningMesage);
  static void LogInfo(const std::string &informationMesage);
};
