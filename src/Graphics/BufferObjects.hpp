#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stdint.h>
#include <vector>

class VertexBufferObject {
private:
  uint32_t m_VBOIndex;

public:
  VertexBufferObject(int usage = GL_STATIC_DRAW);
  ~VertexBufferObject();
  void SetVertices(std::vector<float> &vertices);
  void SetVertices(std::vector<glm::vec3> &vertices);
  void SetVertices(std::vector<glm::vec2> &vertices);
  void SetVertices(float &vertices, uint32_t size);
  void BindVBO();
};

class ElementBufferObject {
private:
  uint32_t m_EBOIndex;

public:
  ElementBufferObject(int usage = GL_STATIC_DRAW);
  ~ElementBufferObject();
  void SetIndices(std::vector<uint32_t> &indices);
  void SetIndices(uint32_t *indices, uint32_t size);
  void BindEBO();
};

class VertexArrayObject {
private:
  uint32_t m_VAOIndex;

public:
  VertexArrayObject();
  ~VertexArrayObject();
  void AddBuffer(uint32_t step, uint32_t offset, uint32_t vertexSize,
                 uint32_t attribIndex);
  void BindVAO();
};
