#include "ExportWindow.hpp"
#include <imgui.h>
#include "../Tools/FileDialog.hpp"
#include "../Locale/Locale.hpp"

ExportWindow::ExportWindow()
    : m_readyToExport(false)
    , m_exportPath("")
    , m_exportFormat(ExportFormat::OBJ) {}

std::optional<ExportData> ExportWindow::GetExportData() {
    if (m_readyToExport) {
        std::filesystem::path path = m_exportPath / m_fileName;
        m_readyToExport = false;
        m_open = false;

        return ExportData { .exportPath = path, .exportFormat = m_exportFormat };
    }

    return std::nullopt;
}

void ExportWindow::Render() {
    if (!m_open)
        return;

    ImGui::Begin(Locale::GetText(LocaleKey::EXPORT_MODEL),
                 &m_open,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
                     | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("%s", Locale::GetText(LocaleKey::SELECT_EXPORT_FORMAT));

    if (ImGui::RadioButton("DAE (Collada)", m_exportFormat == ExportFormat::DAE)) {
        m_exportFormat = ExportFormat::DAE;
    }
    if (ImGui::RadioButton("STL", m_exportFormat == ExportFormat::STL)) {
        m_exportFormat = ExportFormat::STL;
    }
    if (ImGui::RadioButton("STLB", m_exportFormat == ExportFormat::STLB)) {
        m_exportFormat = ExportFormat::STLB;
    }
    if (ImGui::RadioButton("OBJ", m_exportFormat == ExportFormat::OBJ)) {
        m_exportFormat = ExportFormat::OBJ;
    }
    if (ImGui::RadioButton("PLY", m_exportFormat == ExportFormat::PLY)) {
        m_exportFormat = ExportFormat::PLY;
    }
    if (ImGui::RadioButton("PLYB", m_exportFormat == ExportFormat::PLYB)) {
        m_exportFormat = ExportFormat::PLYB;
    }
    if (ImGui::RadioButton("X", m_exportFormat == ExportFormat::X)) {
        m_exportFormat = ExportFormat::X;
    }
    if (ImGui::RadioButton("3DS", m_exportFormat == ExportFormat::_3DS)) {
        m_exportFormat = ExportFormat::_3DS;
    }
    if (ImGui::RadioButton("JSON (for WebGl)", m_exportFormat == ExportFormat::JSON)) {
        m_exportFormat = ExportFormat::JSON;
    }
    if (ImGui::RadioButton("ASSBIN", m_exportFormat == ExportFormat::ASSBIN)) {
        m_exportFormat = ExportFormat::ASSBIN;
    }
    if (ImGui::RadioButton("STEP", m_exportFormat == ExportFormat::STEP)) {
        m_exportFormat = ExportFormat::STEP;
    }
    if (ImGui::RadioButton("glTF 2.0 (partial, experimental)", m_exportFormat == ExportFormat::GLTF2)) {
        m_exportFormat = ExportFormat::GLTF2;
    }
    if (ImGui::RadioButton("glTF 2.0 Binary (partial, experimental)", m_exportFormat == ExportFormat::GLB2)) {
        m_exportFormat = ExportFormat::GLB2;
    }
    if (ImGui::RadioButton("glTF 1.0 (partial, experimental)", m_exportFormat == ExportFormat::GLTF1)) {
        m_exportFormat = ExportFormat::GLTF1;
    }
    if (ImGui::RadioButton("glTF 1.0 Binary (partial, experimental)", m_exportFormat == ExportFormat::GLB1)) {
        m_exportFormat = ExportFormat::GLB1;
    }
    if (ImGui::RadioButton("3MF (experimental)", m_exportFormat == ExportFormat::_3MF)) {
        m_exportFormat = ExportFormat::_3MF;
    }
    if (ImGui::RadioButton("FBX (experimental)", m_exportFormat == ExportFormat::FBX)) {
        m_exportFormat = ExportFormat::FBX;
    }

    ImGui::Separator();

    if (ImGui::Button(Locale::GetText(LocaleKey::SELECT_EXPORT_MODEL_PATH))) {
        m_exportPath = FileDialogManager::GetInstance().InvokeFolderDialog();
    }
    std::string msg = Locale::GetText(LocaleKey::FILE_PATH);
    msg += m_exportPath.empty() ? std::string("None") : m_exportPath.string();

    ImGui::InputText(Locale::GetText(LocaleKey::FILE), m_fileName, IM_ARRAYSIZE(m_fileName));
    ImGui::Text("%s", msg.c_str());
    ImGui::Separator();

    if (ImGui::Button(Locale::GetText(LocaleKey::EXPORT_MODEL))) {
        if (!m_exportPath.empty() && m_fileName[0] != '\0')
            m_readyToExport = true;
    }

    ImGui::End();
}
