#pragma once
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include <glm/glm.hpp>

class ImguiRendering {
public:
  static void InitImgui(GLFWwindow *window);
  static void OnRenderEnd();
  static void ShutdownImguiRendering();
  static void UpdateImgui();
};
