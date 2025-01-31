#include "MessageBox.hpp"
#include <cstdlib>
#include <cstring>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

void ShowMessageBox(const char* title, const char* message, MessageBoxType type) {
#ifdef _WIN32
     UINT uType;
    switch (type) {
    case MessageBoxType::Info:
        uType = MB_ICONINFORMATION;
        break;
    case MessageBoxType::Warning:
        uType = MB_ICONWARNING;
        break;
    case MessageBoxType::Error:
        uType = MB_ICONERROR;
        break;
    }
    MessageBoxA(nullptr, message, title, uType | MB_OK);
#else
    std::string command;

    // Check for kdialog first
    if (std::system("which kdialog > /dev/null 2>&1") == 0) {
        switch (type) {
        case MessageBoxType::Info:
            command = "kdialog --title \"" + std::string(title) + "\" --msgbox \""
                      + std::string(message) + "\"";
            break;
        case MessageBoxType::Warning:
            command = "kdialog --title \"" + std::string(title) + "\" --sorry \""
                      + std::string(message) + "\"";
            break;
        case MessageBoxType::Error:
            command = "kdialog --title \"" + std::string(title) + "\" --error \""
                      + std::string(message) + "\"";
            break;
        }
    }
    // Check for zenity
    else if (std::system("which zenity > /dev/null 2>&1") == 0) {
        switch (type) {
        case MessageBoxType::Info:
            command = "zenity --info --title=\"" + std::string(title) + "\" --text=\""
                      + std::string(message) + "\"";
            break;
        case MessageBoxType::Warning:
            command = "zenity --warning --title=\"" + std::string(title) + "\" --text=\""
                      + std::string(message) + "\"";
            break;
        case MessageBoxType::Error:
            command = "zenity --error --title=\"" + std::string(title) + "\" --text=\""
                      + std::string(message) + "\"";
            break;
        }
    }
    // Check for xmessage
    else if (std::system("which xmessage > /dev/null 2>&1") == 0) {
        switch (type) {
        case MessageBoxType::Info:
        case MessageBoxType::Warning:
        case MessageBoxType::Error:
            command
                = "xmessage -title \"" + std::string(title) + "\" \"" + std::string(message) + "\"";
            break;
        }
    }
    // Fallback to console output if no GUI dialog utility is found
    else {
        command = "echo \"" + std::string(title) + ": " + std::string(message) + "\"";
    }

    std::system(command.c_str());
#endif
}
