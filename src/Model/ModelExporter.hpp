#pragma once
#include "Model.hpp"
#include "assimp/Exporter.hpp"
#include <string>

enum class ExportFormat : uint8_t {
    DAE, // Collada
    STL,
    STLB,
    OBJ,
    PLY,
    PLYB,
    X,
    _3DS,
    JSON,
    ASSBIN,
    STEP,
    GLTF2, // glTF 2.0 (partial)
    GLB2,
    GLTF1,
    GLB1,
    _3MF, // 3MF (experimental)
    FBX   // FBX (experimental)
};

class ModelExporter {
  public:
    ModelExporter();
    ~ModelExporter() = default;
    void ExportModel(const Model& model, const std::string& exportPath, ExportFormat format);

  private:
    Assimp::Exporter m_exporter;
    [[nodiscard]] const char* GetExportFormatExtension(ExportFormat format) const;
};
