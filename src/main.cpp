#include "Core/GLDebug.hpp"
#include "Core/Window.hpp"
#include "GUI/WindowSystem.hpp"
#include "Graphics/Model.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
int main() {
  Window::InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Model Viewer");
  Window::SetVsync(true);
  GLDebug::EnableDebugMode();
  OpenglData::SetFaceCulling(false);
  OpenglData::SetDepthTesting(true);
  WindowSystem windowsSystem;
  
  Model backpack("../../res/backpack/backpack.obj");
  glm::mat4 projection = glm::perspective(
      glm::radians(75.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f,
      1000.0f);
  glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f));
  
  uint32_t texture = loadTexture("../../res/backpack/diffuse.jpg", GL_LINEAR);
  backpack.OverwriteTexture(texture);
  Shader shader("../../src/basic.frag", "../../src/basic.vert");
  shader.SetUniform("projection", projection);
  shader.SetUniform("view", view);
  while (!Window::WindowShouldClose()) {
    
    glm::mat4 model = glm::mat4(1.0f);
    auto inputData = windowsSystem.GetInputData();
    model = glm::translate(model, inputData.modelPos);
    model = glm::rotate(model, glm::radians(inputData.modelRotAngle), inputData.modelRot);
    model = glm::scale(model, inputData.modelScale);
    backpack.SetModelMatrix(model);
    Core::OnRenderStart();
    Core::StartRenderingToTexture(windowsSystem.GetViewportWinSize());

    backpack.DrawArrays(shader);
    Core::FinishRenderingToTexture();
    windowsSystem.RenderWindows();
    Core::OnRenderEnd();
  }
  Window::DestroyWindow();
  return 0;
}
