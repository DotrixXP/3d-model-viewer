#pragma once
#include "../Tools/Log.hpp"
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include <glm/glm.hpp>
#include <map>
#include <string>

class ImguiRendering {
  public:
    static void InitImgui(GLFWwindow* window);
    static void ChangeFontSize(float newSize);
    static void OnRenderEnd();
    static void ShutdownImguiRendering();
    static void UpdateImgui();

  private:
    static int32_t s_fontSize;
    static std::map<int32_t, ImFont*> s_fonts;
};
