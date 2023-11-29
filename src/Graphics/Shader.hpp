#pragma once
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../Tools/Log.hpp"

class Shader {
private:
  std::string m_vertexShaderPath;
  std::string m_fragmentShaderPath;

  GLuint m_shaderProgram;

  std::string m_vertexShaderCode;
  std::string m_fragmentShaderCode;

  void ParseAndCreateShaders();
  uint32_t CompileShader(unsigned int type, const std::string &src);

public:
  Shader(const std::string &fragmentShaderPath,
         const std::string &vertexShaderPath);
  ~Shader();
  GLuint GetShaderProgram();
  void SetUniform(std::string uniformName, int uniformData);
  void SetUniform(std::string uniformName, float uniformData);
  void SetUniform(std::string uniformName, bool uniformData);
  void SetUniform(std::string uniformName, unsigned int uniformData);
  void SetUniform(std::string uniformName, glm::mat4 uniformData);
  void SetUniform(std::string uniformName, glm::mat3 uniformData);
  void SetUniform(std::string uniformName, glm::vec2 uniformData);
  void SetUniform(std::string uniformName, glm::vec3 uniformData);
  void SetUniform(std::string uniformName, glm::vec4 uniformData);
  void Bind();
  friend std::ostream &operator<<(std::ostream &os, const Shader &sh);
};
