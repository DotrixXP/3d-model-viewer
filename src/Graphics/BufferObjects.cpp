#include "BufferObjects.hpp"

VertexBufferObject::VertexBufferObject(int usage) {
  glGenBuffers(1, &m_VBOIndex);
}

VertexBufferObject::~VertexBufferObject() { glDeleteBuffers(1, &m_VBOIndex); }

void VertexBufferObject::SetVertices(std::vector<float> &vertices) {
  glBindBuffer(GL_ARRAY_BUFFER, m_VBOIndex);
  glBufferData(GL_ARRAY_BUFFER, (vertices.size() * sizeof(float)),
               vertices.data(), GL_STATIC_DRAW);
}

void VertexBufferObject::SetVertices(std::vector<glm::vec3> &vertices) {
  glBindBuffer(GL_ARRAY_BUFFER, m_VBOIndex);
  glBufferData(GL_ARRAY_BUFFER, (vertices.size() * sizeof(glm::vec3)),
               vertices.data(), GL_STATIC_DRAW);
}

void VertexBufferObject::SetVertices(std::vector<glm::vec2> &vertices) {
  glBindBuffer(GL_ARRAY_BUFFER, m_VBOIndex);
  glBufferData(GL_ARRAY_BUFFER, (vertices.size() * sizeof(glm::vec2)),
               vertices.data(), GL_STATIC_DRAW);
}

void VertexBufferObject::SetVertices(float &vertices, uint32_t size) {
  glBindBuffer(GL_ARRAY_BUFFER, m_VBOIndex);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, &vertices,
               GL_STATIC_DRAW);
}

void VertexBufferObject::BindVBO() {
  glBindBuffer(GL_ARRAY_BUFFER, m_VBOIndex);
}

ElementBufferObject::ElementBufferObject(int usage) {
  glGenBuffers(1, &m_EBOIndex);
}

ElementBufferObject::~ElementBufferObject() { glDeleteBuffers(1, &m_EBOIndex); }

void ElementBufferObject::SetIndices(std::vector<uint32_t> &indices) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOIndex);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indices.size() * sizeof(uint32_t)),
               indices.data(), GL_STATIC_DRAW);
}

void ElementBufferObject::SetIndices(uint32_t *indices, uint32_t size) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOIndex);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * size, indices,
               GL_STATIC_DRAW);
}

void ElementBufferObject::BindEBO() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOIndex);
}

VertexArrayObject::VertexArrayObject() { glGenVertexArrays(1, &m_VAOIndex); }

VertexArrayObject::~VertexArrayObject() {
  glDeleteVertexArrays(1, &m_VAOIndex);
}

void VertexArrayObject::AddBuffer(uint32_t step, uint32_t offset,
                                  uint32_t vertexSize, uint32_t attribIndex) {
  glBindVertexArray(m_VAOIndex);
  glEnableVertexAttribArray(attribIndex);
  glVertexAttribPointer(attribIndex, vertexSize, GL_FLOAT, GL_FALSE,
                        step * sizeof(float), (void *)offset);
}

void VertexArrayObject::BindVAO() { glBindVertexArray(m_VAOIndex); }
