#include "Shader.hpp"

Shader::Shader(const char *fragmentShaderPath, const char *vertexShaderPath)
{
    m_vertexShaderPath = vertexShaderPath;
    m_fragmentShaderPath = fragmentShaderPath;
    Shader::ParseAndCreateShaders();
}

Shader::~Shader() { glDeleteProgram(m_shaderProgram); }

// Shadery se načtou ze souborů, potom se zkompilují a nalinkují
void Shader::ParseAndCreateShaders()
{
    std::ifstream fragShaderFile;
    std::ifstream vertShaderFile;

    fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        fragShaderFile.open(m_fragmentShaderPath);
        vertShaderFile.open(m_vertexShaderPath);

        std::stringstream vertexShaderCodeStream;
        std::stringstream fragmentShaderCodeStream;

        vertexShaderCodeStream << fragShaderFile.rdbuf();
        fragmentShaderCodeStream << vertShaderFile.rdbuf();

        fragShaderFile.close();
        vertShaderFile.close();

        m_vertexShaderCode = vertexShaderCodeStream.str();
        m_fragmentShaderCode = fragmentShaderCodeStream.str();

        Log::LogInfo("Shadery: " + m_fragmentShaderPath + ' ' +
                     m_vertexShaderPath + " uspesne nacteny!");
    }
    catch (std::ifstream::failure &e)
    {
        Log::LogError("Shader: " + m_fragmentShaderPath + ' ' +
                      m_vertexShaderPath + " " + std::string(e.what()));
        throw e;
        return;
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

uint32_t Shader::CompileShader(uint32_t type, const std::string &shaderPath)
{
    uint32_t id = glCreateShader(type);
    const char *src = shaderPath.c_str();

    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (!result)
    {
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

uint32_t Shader::GetShaderProgram() { return m_shaderProgram; }

// Nastavování veškerých uniformů
//
void Shader::SetUniform(std::string uniformName, int uniformData)
{
    glUseProgram(m_shaderProgram);
    glUniform1i(glGetUniformLocation(m_shaderProgram, uniformName.c_str()),
                uniformData);
}

void Shader::SetUniform(std::string uniformName, uint32_t uniformData)
{
    glUseProgram(m_shaderProgram);
    glUniform1ui(glGetUniformLocation(m_shaderProgram, uniformName.c_str()),
                 uniformData);
}

void Shader::SetUniform(std::string uniformName, glm::mat4 uniformData)
{
    glUseProgram(m_shaderProgram);
    uint32_t uLoc = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
    glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(uniformData));
}

void Shader::SetUniform(std::string uniformName, glm::vec3 uniformData)
{
    glUseProgram(m_shaderProgram);
    uint32_t uLoc = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
    glUniform3f(uLoc, uniformData.x, uniformData.y, uniformData.z);
}

void Shader::SetUniform(std::string uniformName, glm::vec2 uniformData)
{
    glUseProgram(m_shaderProgram);
    uint32_t uLoc = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
    glUniform2f(uLoc, uniformData.x, uniformData.y);
}

void Shader::SetUniform(std::string uniformName, glm::vec4 uniformData)
{
    glUseProgram(m_shaderProgram);
    uint32_t uLoc = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
    glUniform4f(uLoc, uniformData.x, uniformData.y, uniformData.z,
                uniformData.w);
}

void Shader::SetUniform(std::string uniformName, bool uniformData)
{
    glUseProgram(m_shaderProgram);
    glUniform1i(glGetUniformLocation(m_shaderProgram, uniformName.c_str()),
                uniformData);
}

void Shader::SetUniform(std::string uniformName, float uniformData)
{
    glUseProgram(m_shaderProgram);
    glUniform1f(glGetUniformLocation(m_shaderProgram, uniformName.c_str()),
                uniformData);
}

void Shader::Bind() { glUseProgram(m_shaderProgram); }

// Přetížení << pro výpis kódu shaderu pro debugování
std::ostream &operator<<(std::ostream &os, const Shader &sh)
{
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

    return os;
}
