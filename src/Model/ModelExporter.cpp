#include "ModelExporter.hpp"
#include "../Tools/Assert.hpp"
#include "../GUI/ErrorWindow.hpp"

const char* ModelExporter::GetExportFormatExtension(ExportFormat format) const {
    switch (format) {
    case ExportFormat::DAE:
        return "collada";
    case ExportFormat::STL:
        return "stl";
    case ExportFormat::STLB:
        return "stlb";
    case ExportFormat::OBJ:
        return "obj";
    case ExportFormat::PLY:
        return "ply";
    case ExportFormat::PLYB:
        return "plyb";
    case ExportFormat::X:
        return "x";
    case ExportFormat::_3DS:
        return "3ds";
    case ExportFormat::JSON:
        return "assjson";
    case ExportFormat::ASSBIN:
        return "assbin";
    case ExportFormat::STEP:
        return "stp";
    case ExportFormat::GLTF2:
        return "gltf2";
    case ExportFormat::GLB2:
        return "glb2";
    case ExportFormat::GLTF1:
        return "gltf";
    case ExportFormat::GLB1:
        return "glb";
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

    try {

        if (m_exporter.Export(scene, formatExtension, fullPath) != AI_SUCCESS) {
            Log::Error("Failed to export model: " + std::string(m_exporter.GetErrorString()));
            GuiErrorWindow::RaiseError(
                "Failed to export model, scene is empty. This is a bug in exporting engine.");
        }
        else {
            Log::Info("Model exported successfully to: " + fullPath);
        }
    }
    catch (const std::exception& e) {
        Log::Error("Failed to export model: " + std::string(e.what()));
        GuiErrorWindow::RaiseError(("Model exporting failed! " + std::string(e.what())).c_str());
        PANIC(("Model exporting failed! " + std::string(e.what())).c_str());
    }
}
