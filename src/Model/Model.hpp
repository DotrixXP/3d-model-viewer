#pragma once
#include "../Tools/Log.hpp"
#include "Mesh.hpp"
#include "../Texture/Texture.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>
#include <memory>
#include <optional>

class ModelExporter;

class Model {
  public:
    void DrawArrays() const;
    [[nodiscard]] uint32_t GetVerticesCount() const;
    [[nodiscard]] uint32_t GetIndicesCount() const;
    [[nodiscard]] Material& GetMaterials();
    [[nodiscard]] const std::optional<glm::vec4> GetColor() const;
    [[nodiscard]] const std::filesystem::path& GetDirectoryPath() const;

    ~Model();
    Model(const Model& other) = delete;
    Model(Model&& other) noexcept;

  private:
    Model(
        std::vector<Mesh>&& meshes, const aiScene* scene, const std::filesystem::path modelPath);
    void LoadModel(const std::string& directoryPath);
    void ProcessNode(aiNode* node, const aiScene* scene);

    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::filesystem::path m_path;
    std::vector<Mesh> m_meshes;
    const aiScene* m_scene;
    Material m_materials;
    glm::mat4 m_model;

    friend class ModelExporter;
    friend class ModelLoader;
};
