#pragma once
#include "../Tools/Log.hpp"
#include "Mesh.hpp"
#include "TextureLoader.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Model
{
  private:
    bool m_manualySetTextures;
    std::string m_directoryPath; // Cesta k modelu
    std::vector<Mesh> m_meshes;  // vektor kompletních Meshů
    std::vector<MeshData::Texture> m_loadedTextures;
    std::vector<std::string> m_loadedTexturesPaths; // Cesty načtených textur
    glm::vec4 m_color;
    void LoadModel(std::string directoryPath);            // Načíst model
    void ProcessNode(aiNode *node, const aiScene *scene); // Získání nodů
    Mesh ProcessMesh(aiMesh *mesh,
                     const aiScene *scene); // Získání vertexů z nodů
    std::vector<MeshData::Texture>
    LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
                         std::string typeName); // Získání Textur
    glm::mat4 m_model;

  public:
    Model(const std::string &directoryPath,
          bool manualySetTextures =
              false); // Konstruktor s cestou k modelu jako argument
    const void DrawArrays(Shader &shader) const;
    void OverwriteTexture(uint32_t texture);
    void OverwriteColor(glm::vec3 color);
    const uint32_t GetVerticesCount() const;
    const uint32_t GetIndicesCount() const;
    const glm::vec4 GetColor() const;
    const std::string &GetDirectoryPath() const;
    ~Model();
};
