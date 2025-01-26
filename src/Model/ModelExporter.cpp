#include "ModelExporter.hpp"
#include "../Tools/Assert.hpp"
#include "../GUI/ErrorWindow.hpp"

const char* ModelExporter::GetExportFormatExtension(ExportFormat format) const {
    switch (format) {
    case ExportFormat::DAE:
        return "dae";
    case ExportFormat::STL:
        return "stl";
    case ExportFormat::OBJ:
        return "obj";
    case ExportFormat::PLY:
        return "ply";
    case ExportFormat::X:
        return "x";
    case ExportFormat::_3DS:
        return "3ds";
    case ExportFormat::JSON:
        return "json";
    case ExportFormat::ASSBIN:
        return "assbin";
    case ExportFormat::STEP:
        return "step";
    case ExportFormat::PBRTv4:
        return "pbrt";
    case ExportFormat::GLTF2:
        return "gltf";
    case ExportFormat::_3MF:
        return "3mf";
    case ExportFormat::FBX:
        return "fbx";
    default:
        PANIC("Unsupported export format, this is a bug!");
    }
}

ModelExporter::ModelExporter()
    : m_exporter() {}

void ModelExporter::ExportModel(const Model& model,
                                const std::string& exportPath,
                                ExportFormat format) {
    const aiScene* scene = model.m_scene;

    if (!scene) {
        GuiErrorWindow::RaiseError("Failed to export model, scene is empty. This is a bug.");
        return;
    }

    const char* formatExtension = GetExportFormatExtension(format);
    std::string fullPath = exportPath + "." + formatExtension;

    if (m_exporter.Export(scene, formatExtension, fullPath) != AI_SUCCESS) {
        Log::Error("Failed to export model: " + std::string(m_exporter.GetErrorString()));
        GuiErrorWindow::RaiseError("Failed to export model, scene is empty. This is a bug.");
    }
    else {
        Log::Info("Model exported successfully to: " + fullPath);
    }
}
