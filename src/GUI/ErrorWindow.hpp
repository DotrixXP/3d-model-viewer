#pragma once

#include <string>
class GuiErrorWindow {
  public:
    GuiErrorWindow() = delete;
    ~GuiErrorWindow() = delete;

    static void RaiseError(const std::string& errorMessage);

  private:
    static void Render();
    static std::string s_errorMessage;
    static bool s_open;

    friend class WindowSystem;
};
