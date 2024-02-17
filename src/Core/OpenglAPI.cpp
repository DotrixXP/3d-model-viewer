#include "OpenglAPI.hpp"

uint32_t OpenglAPI::m_drawCallsCount = 0;
uint32_t OpenglAPI::m_drawCallsCountValue = OpenglAPI::m_drawCallsCount;

bool OpenglAPI::InitOpenglAPI()
{

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Log::LogError("OpenglAPI (Glad) se nespustil!");
        return false;
    }
    else
        return true;
}

void OpenglAPI::DrawArrays(GLenum mode, GLenum verticesCount)
{
    OpenglAPI::m_drawCallsCount++;
    glDrawArrays(mode, 0, verticesCount);
}

void OpenglAPI::DrawElements(GLenum mode, GLenum indicesCount)
{
    OpenglAPI::m_drawCallsCount++;
    glDrawElements(mode, indicesCount, GL_UNSIGNED_INT, nullptr);
}

void OpenglAPI::OnRenderEnd()
{
    OpenglAPI::m_drawCallsCountValue = OpenglAPI::m_drawCallsCount;
    OpenglAPI::m_drawCallsCount = 0;
}

uint32_t OpenglAPI::GetDrawCallsCount() { return m_drawCallsCountValue; }
