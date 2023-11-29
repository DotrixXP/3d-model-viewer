#pragma once
#include "../Tools/Log.hpp"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>
#define CheckForError() GLDebug::CheckForError_(__FILE__, __LINE__)
#define CheckForErrorInfo()                                                    \
  glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR,              \
                        GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id,
                            GLenum severity, GLsizei length,
                            const char *message, const void *userParam);

class GLDebug {
private:
  uint32_t errorCount;

public:
  GLDebug() = delete;
  GLDebug(const GLDebug &) = delete;
  GLDebug &operator=(const GLDebug &) = delete;
  static void EnableDebugMode();
  static GLenum CheckForError_(const char *file, int line);
};
