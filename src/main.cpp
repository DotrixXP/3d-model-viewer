#include "Core/Window.hpp"
#include "Tools/Log.hpp"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include "GUI/WindowSystem.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
int main() {
  Window::InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Model Viewer");
  WindowSystem windowsSystem;
  while (!Window::WindowShouldClose()) {
    Core::OnRenderStart();
    windowsSystem.RenderWindows();
    Core::OnRenderEnd();
  }
  Window::DestroyWindow();
  return 0;
}
