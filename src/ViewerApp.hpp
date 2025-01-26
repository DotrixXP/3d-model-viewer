#include "Core/GLDebug.hpp"
#include "Core/Window.hpp"
#include "GUI/ErrorWindow.hpp"
#include "GUI/WindowSystem.hpp"
#include "Model/Model.hpp"
#include "Graphics/Renderer.hpp"
#include "Model/ModelHandler.hpp"
#include "Model/ModelLoader.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class ViewerApp {
  public:
    ViewerApp() = default;
    ~ViewerApp() = default;

    void Run(const std::string& initialModelPath);
};

inline void ViewerApp::Run(const std::string& initialModelPath) {
    std::string modelPath = initialModelPath;
    Window::InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Model Viewer");
    Window::SetVsync(true);
    GLDebug::EnableDebugMode();
    OpenglData::SetFaceCulling(false);
    OpenglData::SetDepthTesting(true);
    WindowSystem windowsSystem;

    ModelHandler modelHandler;
    while (!Window::WindowShouldClose()) {
        auto inputData = windowsSystem.GetInputData();
        if (modelPath.size() > 0) {
            WindowSystem::s_newModelPath = std::filesystem::path(initialModelPath);
            modelPath.clear();
        }

        Core::OnRenderStart();
        Core::StartRenderingToTexture(windowsSystem.GetViewportWinSize());
        CameraSystem::GetInstance().UpdateInput();
        CameraSystem::GetInstance().SetInputState(inputData.GetAllowCameraInput());
        modelHandler.LoadSelectedModel();

        modelHandler.RenderSelectedModel(inputData);

        if (inputData.GetSkyboxActive())
            Renderer::GetInstance().RenderAdditional();

        auto exportData = windowsSystem.GetExportData();
        if (exportData.has_value())
            modelHandler.ExportSelectedModel(exportData.value());

        Renderer::GetInstance().SetActiveShader(inputData.GetActiveShader());
        Renderer::GetInstance().SetLightIntensity(inputData.GetLightIntensity());
        Renderer::GetInstance().SetPolygonMode(inputData.GetPolygonMode());
        windowsSystem.ApplyGuiData();
        PerfData::CollectPerformanceData();
        Core::FinishRenderingToTexture();
        windowsSystem.RenderWindows(modelHandler.IsModelLoaded());
        Core::OnRenderEnd();
    }
    Window::DestroyWindow();
}
