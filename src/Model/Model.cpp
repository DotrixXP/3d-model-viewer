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

void Model::ExportModel(const std::string& exportPath, const std::string& format) const {
    Assimp::Exporter exporter;

    // Create a new scene to export
    aiScene scene;
    scene.mRootNode = new aiNode();

    // Fill the scene with meshes
    scene.mMeshes = new aiMesh*[m_meshes.size()];
    scene.mNumMeshes = m_meshes.size();
    scene.mMaterials = new aiMaterial*[1];
    scene.mMaterials[0] = new aiMaterial();
    scene.mNumMaterials = 1;

    for (size_t i = 0; i < m_meshes.size(); ++i) {
        const auto& mesh = m_meshes[i];
        auto* ai_mesh = new aiMesh();
        ai_mesh->mMaterialIndex = 0;

        // Fill vertices
        ai_mesh->mVertices = new aiVector3D[mesh.vertices.size()];
        ai_mesh->mNumVertices = mesh.vertices.size();
        for (size_t j = 0; j < mesh.vertices.size(); ++j) {
            ai_mesh->mVertices[j] = aiVector3D(mesh.vertices[j].position.x,
                                               mesh.vertices[j].position.y,
                                               mesh.vertices[j].position.z);
        }

        // Fill indices
        ai_mesh->mFaces = new aiFace[mesh.indices.size() / 3];
        ai_mesh->mNumFaces = mesh.indices.size() / 3;
        for (size_t j = 0; j < mesh.indices.size() / 3; ++j) {
            aiFace& face = ai_mesh->mFaces[j];
            face.mIndices = new unsigned int[3];
            face.mNumIndices = 3;
            face.mIndices[0] = mesh.indices[j * 3];
            face.mIndices[1] = mesh.indices[j * 3 + 1];
            face.mIndices[2] = mesh.indices[j * 3 + 2];
        }

        scene.mMeshes[i] = ai_mesh;
    }

    // Export the scene
    if (exporter.Export(&scene, format, exportPath) != AI_SUCCESS) {
        Log::Error("Failed to export model: " + std::string(exporter.GetErrorString()));
    }
    else {
        Log::Info("Model exported successfully to: " + exportPath);
    }

    // Clean up
    for (size_t i = 0; i < scene.mNumMeshes; ++i) {
        delete[] scene.mMeshes[i]->mVertices;
        for (size_t j = 0; j < scene.mMeshes[i]->mNumFaces; ++j) {
            delete[] scene.mMeshes[i]->mFaces[j].mIndices;
        }
        delete[] scene.mMeshes[i]->mFaces;
        delete scene.mMeshes[i];
    }
    delete[] scene.mMeshes;
    delete scene.mRootNode;
    delete scene.mMaterials[0];
    delete[] scene.mMaterials;
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
