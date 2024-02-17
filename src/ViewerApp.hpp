#include "Core/GLDebug.hpp"
#include "Core/Window.hpp"
#include "GUI/WindowSystem.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/Renderer.hpp"
#include "Tools/ModelLoader.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class ViewerApp
{
  public:
    ViewerApp() = default;
    ~ViewerApp() = default;

    void Run();
};

inline void ViewerApp::Run()
{
    Window::InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Model Viewer");
    Window::SetVsync(true);
    GLDebug::EnableDebugMode();
    OpenglData::SetFaceCulling(false);
    OpenglData::SetDepthTesting(true);
    WindowSystem windowsSystem;

    ModelLoader modelLoader;

    while (!Window::WindowShouldClose())
    {
        auto inputData = windowsSystem.GetInputData();
        Core::OnRenderStart();

        Core::StartRenderingToTexture(windowsSystem.GetViewportWinSize());
        CameraSystem::GetInstance().UpdateInput();
        modelLoader.LoadSelectedModel();
        modelLoader.RenderSelectedModel(inputData);
        windowsSystem.ApplyGuiData();
        PerfData::CollectPerformanceData();
        Core::FinishRenderingToTexture();
        windowsSystem.RenderWindows(modelLoader.IsModelLoaded());
        Core::OnRenderEnd();
    }
    Window::DestroyWindow();
}
