#pragma once
#include <cstdint>

enum class MessageBoxType : uint8_t {
    Info,
    Warning,
    Error
};

void ShowMessageBox(const char* title,
                    const char* message,
                    MessageBoxType type = MessageBoxType::Info);
