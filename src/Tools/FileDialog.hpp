#pragma once
#include <nfd.h>
#include "Log.hpp"
#include <memory>

class FileDialogManager {
public:
    static void Init();
    static FileDialogManager &GetInstance();
    FileDialogManager(FileDialogManager const &) = delete;
    void operator=(FileDialogManager const &) = delete;

    ~FileDialogManager();
    const std::string InvokeFileDialog();
private:
    static std::unique_ptr<FileDialogManager> s_instance;
    FileDialogManager();
};
