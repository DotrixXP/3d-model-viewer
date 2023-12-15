#include "FileDialog.hpp"

std::unique_ptr<FileDialogManager> FileDialogManager::s_instance = nullptr;

FileDialogManager &FileDialogManager::GetInstance() {
  static FileDialogManager instance;
  return instance;
}

FileDialogManager::FileDialogManager() { NFD_Init(); }

FileDialogManager::~FileDialogManager() { NFD_Quit(); }

const std::string FileDialogManager::InvokeFileDialog() {
  nfdchar_t *outPath = nullptr;
  nfdresult_t result = NFD_OpenDialog(&outPath, nullptr, 0, nullptr);
  std::string path;
  if (result == NFD_OKAY) {
    path = std::string(outPath);
    free(outPath);
  } else if (result == NFD_CANCEL) {
    Log::LogError("User pressed cancel.");
  } else {
    Log::LogError("Error: %s\n" + std::string(NFD_GetError()));
  }
  return path;
}
