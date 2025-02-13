#include "Mesh.hpp"

#include <utility>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
    : m_VBO(0)
    , m_EBO(0)
    , m_VAO(0)
    , vertices(std::move(vertices))
    , indices(std::move(indices)) {
    SetMesh();
}

void Mesh::SetMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);             // Bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Bind VBO

    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(Vertex),
                 vertices.data(),
                 GL_STATIC_DRAW); // Data to VBO

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO); // Bind EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(),
                 GL_STATIC_DRAW); // Data to EBO

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // TexCoord attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    // Tangent attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    // Bitangent attribute
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
        4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0); // Unbind VAO
}

void Mesh::DrawArrays() const {
    glBindVertexArray(m_VAO);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)nullptr);
}

void Mesh::Destroy() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_VAO);
}
