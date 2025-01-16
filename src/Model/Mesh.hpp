#pragma once
#include "../Graphics/Shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
  private:
    unsigned int m_VBO, m_EBO, m_VAO;
    void SetMesh();

  public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

    void DrawArrays() const;
    void Destroy();
};
