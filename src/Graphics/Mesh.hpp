#pragma once
#include "../Graphics/Shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <assimp/Importer.hpp>
#include <string>
#include <vector>

namespace MeshData {

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string directoryPath;
};
} // namespace MeshData

class Mesh {
private:
  unsigned int m_VBO, m_EBO, m_VAO;
  void SetMesh();

public:
  std::vector<MeshData::Vertex> vertices;
  std::vector<MeshData::Texture> textures;
  std::vector<unsigned int> indices;

  Mesh(std::vector<MeshData::Vertex> vertices,
       std::vector<unsigned int> indices,
       std::vector<MeshData::Texture> textures);
  void DrawArrays(Shader &shader, bool manualySetTextures);
  void Destroy();
};
