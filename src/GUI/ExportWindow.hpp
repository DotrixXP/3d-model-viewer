#pragma once
#include <filesystem>
#include "../Model/ModelExporter.hpp"

class WindowSystem;

struct ExportData {
    std::filesystem::path exportPath;
    ExportFormat exportFormat;
};

class ExportWindow {
  public:
    ExportWindow();
    ~ExportWindow() = default;
    void Render();
    [[nodiscard]] std::optional<ExportData> GetExportData();

  private:
    bool m_readyToExport;
    bool m_open = false;
    std::filesystem::path m_exportPath;
    char m_fileName[256] = "";
    ExportFormat m_exportFormat;

    friend WindowSystem;
};
