#pragma once
#include "../Tools/Log.hpp"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <stdint.h>

class OpenglAPI {
public:
  OpenglAPI() = delete;
  static bool InitOpenglAPI();
  static void DrawArrays(GLenum mode, GLenum verticesCount);
  static void DrawElements(GLenum mode, GLenum indicesCount);
  static void OnRenderEnd();
  static uint32_t GetDrawCallsCount();

private:
  static uint32_t m_drawCallsCountValue;
  static uint32_t m_drawCallsCount;
};
