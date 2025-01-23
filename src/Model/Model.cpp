#include "Model.hpp"

#include <assimp/Exporter.hpp>

#include <utility>

Model::Model(std::vector<Mesh>&& meshes,
             const aiScene* scene,
             const std::filesystem::path modelPath)
    : m_path(modelPath)
    , m_meshes(std::move(meshes))
    , m_scene(scene)
    , m_model(glm::mat4(1.f)) {}

void Model::DrawArrays() const {
    if (m_materials.IsValid())
        for (const auto& m_meshe : m_meshes)
            m_meshe.DrawArrays();
}

const std::filesystem::path& Model::GetDirectoryPath() const {
    return m_path;
}

Model::~Model() {
    if (m_meshes.empty())
        return;

    for (auto mesh : m_meshes)
        mesh.Destroy();

    Log::Info("Model destroyed");
}

Model::Model(Model&& other) noexcept
    : m_meshes(std::move(other.m_meshes))
    , m_scene(other.m_scene)
    , m_materials(other.m_materials)
    , m_model(other.m_model) {
    other.m_scene = nullptr;
    other.m_materials = Material();
    other.m_model = glm::mat4(1.f);
    other.m_meshes.clear();
}

Material& Model::GetMaterials() {
    return m_materials;
}

uint32_t Model::GetVerticesCount() const {
    uint32_t vertexCount = 0;
    for (const auto& m_meshe : m_meshes) {
        vertexCount += m_meshe.vertices.size();
    }
    return vertexCount;
}

uint32_t Model::GetIndicesCount() const {
    uint32_t indicesCount = 0;
    for (const auto& m_meshe : m_meshes) {
        indicesCount += m_meshe.indices.size();
    }
    return indicesCount;
}
