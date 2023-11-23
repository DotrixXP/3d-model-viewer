#include "Log.hpp"

// bool Log::logToFile = false;

void Log::LogError(const std::string &errorMesage) {
  std::cout << "[Log System] ";

#ifdef PLATFORM_WINDOWS
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(h, 4);
#endif

  std::cout << "Error! " << errorMesage << std::endl;

#ifdef PLATFORM_WINDOWS
  SetConsoleTextAttribute(h, 15);
#endif
}

void Log::LogWarning(const std::string &warningMesage) {
  std::cout << "[Log System] ";

#ifdef PLATFORM_WINDOWS
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(h, 14);
#endif

  std::cout << "Warning! " << warningMesage << std::endl;

#ifdef PLATFORM_WINDOWS
  SetConsoleTextAttribute(h, 15);
#endif
}

void Log::LogInfo(const std::string &informationMesage) {
  std::cout << "[Log System] ";

#ifdef PLATFORM_WINDOWS
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(h, 9);
#endif

  std::cout << "Info! " << informationMesage << std::endl;

#ifdef PLATFORM_WINDOWS
  SetConsoleTextAttribute(h, 15);
#endif
}
