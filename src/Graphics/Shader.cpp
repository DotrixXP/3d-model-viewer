#include "Shader.hpp"

Shader::Shader(const std::string &fragmentShaderPath,
               const std::string &vertexShaderPath) {
  m_vertexShaderPath = vertexShaderPath;
  m_fragmentShaderPath = fragmentShaderPath;
  m_geometryShaderPath = "";
  Shader::ParseAndCreateTwoShaders();
}

Shader::Shader(const std::string &fragmentShaderPath,
               const std::string &vertexShaderPath,
               const std::string &geometryShaderPath) {
  m_vertexShaderPath = vertexShaderPath;
  m_fragmentShaderPath = fragmentShaderPath;
  m_geometryShaderPath = geometryShaderPath;
  Shader::ParseAndCreateThreeShaders();
}

Shader::~Shader() { glDeleteProgram(m_shaderProgram); }

void Shader::ParseAndCreateTwoShaders() {
  std::ifstream vertexShaderFile;
  std::ifstream fragmentShaderFile;

  vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    vertexShaderFile.open(m_fragmentShaderPath);
    fragmentShaderFile.open(m_vertexShaderPath);

    std::stringstream vertexShaderCodeStream;
    std::stringstream fragmentShaderCodeStream;

    vertexShaderCodeStream << vertexShaderFile.rdbuf();
    fragmentShaderCodeStream << fragmentShaderFile.rdbuf();

    vertexShaderFile.close();
    fragmentShaderFile.close();

    m_vertexShaderCode = vertexShaderCodeStream.str();
    m_fragmentShaderCode = fragmentShaderCodeStream.str();

    Log::LogInfo("Shadery: " + m_fragmentShaderPath + ' ' + m_vertexShaderPath +
                 " uspesne nacteny!");
  } catch (std::ifstream::failure &e) {
    Log::LogError("Shader: " + m_fragmentShaderPath + ' ' + m_vertexShaderPath +
                  " " + std::string(e.what()));
  }

  m_shaderProgram = glCreateProgram();
  uint32_t vs = Shader::CompileShader(GL_FRAGMENT_SHADER, m_vertexShaderCode);
  uint32_t fs = Shader::CompileShader(GL_VERTEX_SHADER, m_fragmentShaderCode);

  glAttachShader(m_shaderProgram, vs);
  glAttachShader(m_shaderProgram, fs);
  glLinkProgram(m_shaderProgram);
  glValidateProgram(m_shaderProgram);

  glDeleteShader(vs);
  glDeleteShader(fs);
}

void Shader::ParseAndCreateThreeShaders() {
  std::ifstream vertexShaderFile;
  std::ifstream fragmentShaderFile;
  std::ifstream geometryShaderFile;

  vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  geometryShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    vertexShaderFile.open(m_fragmentShaderPath);
    fragmentShaderFile.open(m_vertexShaderPath);
    geometryShaderFile.open(m_geometryShaderPath);

    std::stringstream vertexShaderCodeStream;
    std::stringstream fragmentShaderCodeStream;
    std::stringstream geometryShaderCodeStream;

    vertexShaderCodeStream << vertexShaderFile.rdbuf();
    fragmentShaderCodeStream << fragmentShaderFile.rdbuf();
    geometryShaderCodeStream << geometryShaderFile.rdbuf();

    vertexShaderFile.close();
    fragmentShaderFile.close();
    geometryShaderFile.close();

    m_vertexShaderCode = vertexShaderCodeStream.str();
    m_fragmentShaderCode = fragmentShaderCodeStream.str();
    m_geometryShaderCode = geometryShaderCodeStream.str();

  } catch (std::ifstream::failure &e) {
    Log::LogError("Shader: " + std::string(e.what()));
  }

  m_shaderProgram = glCreateProgram();
  uint32_t vs = Shader::CompileShader(GL_FRAGMENT_SHADER, m_vertexShaderCode);
  uint32_t fs = Shader::CompileShader(GL_VERTEX_SHADER, m_fragmentShaderCode);
  uint32_t gs = Shader::CompileShader(GL_GEOMETRY_SHADER, m_geometryShaderCode);

  glAttachShader(m_shaderProgram, vs);
  glAttachShader(m_shaderProgram, fs);
  glAttachShader(m_shaderProgram, gs);
  glLinkProgram(m_shaderProgram);
  glValidateProgram(m_shaderProgram);

  glDeleteShader(vs);
  glDeleteShader(fs);
  glDeleteShader(gs);
}

unsigned int Shader::CompileShader(unsigned int type,
                                   const std::string &shaderPath) {
  unsigned int id = glCreateShader(type);
  const char *src = shaderPath.c_str();

  glShaderSource(id, 1, &src, NULL);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (!result) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = new char[length];
    glGetShaderInfoLog(id, length, &length, message);
    Log::LogError("Shader: compilation failed!" + std::string(message));

    delete[] message;
    glDeleteShader(id);
  }

  return id;
}

GLuint Shader::GetShaderProgram() { return m_shaderProgram; }

void Shader::SetUniform(std::string uniformName, int uniformData) {
  glUseProgram(m_shaderProgram);
  glUniform1i(glGetUniformLocation(m_shaderProgram, uniformName.c_str()),
              uniformData);
}

void Shader::SetUniform(std::string uniformName, unsigned int uniformData) {
  glUseProgram(m_shaderProgram);
  glUniform1ui(glGetUniformLocation(m_shaderProgram, uniformName.c_str()),
               uniformData);
}

void Shader::SetUniform(std::string uniformName, glm::mat3 uniformData) {
  glUseProgram(m_shaderProgram);
  unsigned int uLoc =
      glGetUniformLocation(m_shaderProgram, uniformName.c_str());
  glUniformMatrix3fv(uLoc, 1, GL_FALSE, glm::value_ptr(uniformData));
}

void Shader::SetUniform(std::string uniformName, glm::mat4 uniformData) {
  glUseProgram(m_shaderProgram);
  unsigned int uLoc =
      glGetUniformLocation(m_shaderProgram, uniformName.c_str());
  glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(uniformData));
}

void Shader::SetUniform(std::string uniformName, glm::vec3 uniformData) {
  glUseProgram(m_shaderProgram);
  unsigned int uLoc =
      glGetUniformLocation(m_shaderProgram, uniformName.c_str());
  glUniform3f(uLoc, uniformData.x, uniformData.y, uniformData.z);
}

void Shader::SetUniform(std::string uniformName, glm::vec2 uniformData) {
  glUseProgram(m_shaderProgram);
  unsigned int uLoc =
      glGetUniformLocation(m_shaderProgram, uniformName.c_str());
  glUniform2f(uLoc, uniformData.x, uniformData.y);
}

void Shader::SetUniform(std::string uniformName, glm::vec4 uniformData) {
  glUseProgram(m_shaderProgram);
  unsigned int uLoc =
      glGetUniformLocation(m_shaderProgram, uniformName.c_str());
  glUniform4f(uLoc, uniformData.x, uniformData.y, uniformData.z, uniformData.w);
}

void Shader::SetUniform(std::string uniformName, bool uniformData) {
  glUseProgram(m_shaderProgram);
  glUniform1i(glGetUniformLocation(m_shaderProgram, uniformName.c_str()),
              uniformData);
}

void Shader::SetUniform(std::string uniformName, float uniformData) {
  glUseProgram(m_shaderProgram);
  glUniform1f(glGetUniformLocation(m_shaderProgram, uniformName.c_str()),
              uniformData);
}

void Shader::Bind() { glUseProgram(m_shaderProgram); }

std::ostream &operator<<(std::ostream &os, const Shader &sh) {
  os << std::endl;
  os << "/Begin of Vertex Shader Code/"
     << "\n"
     << sh.m_fragmentShaderCode << std::endl
     << "/End of Vertex Shader Code/" << std::endl;

  os << std::endl << std::endl;

  os << "/Begin of Fragment Shader Code/"
     << "\n"
     << sh.m_vertexShaderCode << std::endl
     << "/End of Fragment Shader Code/" << std::endl;

  if (sh.m_geometryShaderPath != "") {
    os << std::endl << std::endl;

    os << "/Begin of Geometry Shader Code/"
       << "\n"
       << sh.m_geometryShaderCode << std::endl
       << "/End of Geometry Shader Code/" << std::endl;
  }

  return os;
}
