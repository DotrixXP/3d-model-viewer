
#include "Core/GLDebug.hpp"
#include "Core/Window.hpp"
#include "GUI/WindowSystem.hpp"
#include "Graphics/Model.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Graphics/Renderer.hpp"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class ViewerApp 
{
public:
    ViewerApp() = default;
    ~ViewerApp() = default;
       
    void Run();
};

inline void ViewerApp::Run() {
  Window::InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Model Viewer");
  Window::SetVsync(true);
  GLDebug::EnableDebugMode();
  OpenglData::SetFaceCulling(false);
  OpenglData::SetDepthTesting(true);
  WindowSystem windowsSystem;

  Model backpack("res/backpack/backpack.obj");

  uint32_t texture = loadTexture("res/backpack/diffuse.jpg", GL_LINEAR);
  backpack.OverwriteTexture(texture);

  while (!Window::WindowShouldClose()) {
    auto inputData = windowsSystem.GetInputData();
    Core::OnRenderStart();
    
    Core::StartRenderingToTexture(windowsSystem.GetViewportWinSize());
    CameraSystem::GetInstance().UpdateInput();
    Renderer::GetInstance().RenderModel(backpack, inputData);
    windowsSystem.ApplyGuiData();
    PerfData::CollectPerformanceData();
    Core::FinishRenderingToTexture();
    windowsSystem.RenderWindows();
    Core::OnRenderEnd();
  }
  Window::DestroyWindow();
}


