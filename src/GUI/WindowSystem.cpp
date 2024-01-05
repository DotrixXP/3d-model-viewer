#include "WindowSystem.hpp"
#include "../Graphics/Renderer.hpp"
#include "imgui.h"

glm::vec2 WindowSystem::s_viewportWinSize = glm::vec2(-1.0f);

InputData::InputData(glm::vec3 modelPos, glm::vec3 modelRot,
                     glm::vec3 modelScale, float modelRotAngle)
    : modelPos(modelPos), modelRot(modelRot), modelScale(modelScale),
      modelRotAngle(modelRotAngle) {}

CameraSettings::CameraSettings() noexcept
    : cameraType(0), projectionType(2), fov(45.0f) {}
WindowSystem::WindowSystem()
    : m_inputData(glm::vec3(0.f, 0.f, -5.f), glm::vec3(1.0f), glm::vec3(1.0f), 0.0f) {
  s_viewportWinSize = glm::vec2(uint32_t(500), uint32_t(500));
}
void WindowSystem::RenderWindows() {
  ImVec2 mainMenuBarSize = this->RenderMainMenuBar();

  ImVec2 screenSize = ImGui::GetIO().DisplaySize;

  ImGui::Begin("Viewport", nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove);
  ImGui::SetWindowSize(
      ImVec2(screenSize.x / 1.5, screenSize.y - mainMenuBarSize.y));
  ImGui::SetWindowPos(ImVec2(0, mainMenuBarSize.y));
  auto winSize = ImGui::GetWindowSize();
  winSize.y -= 40;
  if (winSize.x > 0 && winSize.y > 0)
    s_viewportWinSize = glm::vec2(winSize.x, winSize.y);
  ImGui::Image((void *)(intptr_t)Core::GetRenderTargetTexture(), winSize);
  ImGui::End();

  ImGui::Begin("Vlastnosti", nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove);
  RenderPositionsWidgets();
  RenderClearColorPicker();
  RenderModelInfo();
  RenderCameraSettings();
  ImGui::Text("Zde budou další vlastnosti a nastavení...");

  ImGui::SetWindowSize(ImVec2(screenSize.x - screenSize.x / 1.5,
                              screenSize.y - mainMenuBarSize.y));
  ImGui::SetWindowPos(ImVec2(screenSize.x / 1.5, mainMenuBarSize.y));
  ImGui::End();
}

ImVec2 WindowSystem::RenderMainMenuBar() {
  ImVec2 mainMenuBarSize = ImGui::GetIO().DisplaySize;
  if (ImGui::BeginMainMenuBar()) {
    mainMenuBarSize = ImGui::GetWindowSize();
    if (ImGui::BeginMenu("Soubor")) {
      if (ImGui::MenuItem("Otevrit", "Ctrl+O")) {
        Log::LogInfo(
            std::string(FileDialogManager::GetInstance().InvokeFileDialog()));
      }
      if (ImGui::MenuItem("Zavrit",
                          "Ctrl+W")) { // TODO: Funkce pro zavreni aplikace
        Window::DestroyWindow();       // TODO: Hází výjímku - vyřešit
      }
      ImGui::EndMenu();
    }
    if (m_cameraSettings.cameraType == 1) {
      ImGui::Separator();
      ImGui::Text("Pohyb kamery pomocí WASD");
      ImGui::Text("Pro odemčení kurzoru stiskněte F1, pro zamčení F2");
    }
    ImGui::EndMainMenuBar();
  }
  return mainMenuBarSize;
}

void WindowSystem::RenderPositionsWidgets() {
  ImGui::SliderFloat3("Pozice", &m_inputData.modelPos.x, -10.0f, 10.0f);
  ImGui::SliderFloat3("Rotace", &m_inputData.modelRot.x, -1.0f, 1.0f);
  ImGui::SliderFloat3("Velikost", &m_inputData.modelScale.x, 0.0f, 10.0f);
  ImGui::SliderFloat("Uhel rotace", &m_inputData.modelRotAngle, 0.0f, 360.0f);
}

void WindowSystem::RenderClearColorPicker() {
  static ImVec4 color = ImVec4(1.f, 1.f, 1.f, 1.f);
  ImGui::ColorEdit3("Barva pozadí", (float *)&color);
  Core::SetBackgroundColor(glm::vec3(color.x, color.y, color.z));
}

void WindowSystem::RenderModelInfo() {
  ImGui::Separator();
  ImGui::Text("Model info");
  ImGui::Text("Počet vertexů: %d", Renderer::GetInstance().GetVerticesCount());
  ImGui::Text("Počet indexů: %d", Renderer::GetInstance().GetIndicesCount());
  ImGui::Separator();
}

void WindowSystem::RenderCameraSettings() {
  ImGui::Text("Nastavení kamery");
  if (ImGui::RadioButton("Freefly kamera", m_cameraSettings.cameraType == 1))
    m_cameraSettings.cameraType = 1;
  if (ImGui::RadioButton("Statická kamera", m_cameraSettings.cameraType == 0))
    m_cameraSettings.cameraType = 0;
  if (m_cameraSettings.cameraType == 1) {
    ImGui::BeginDisabled();
  }
  ImGui::Separator();
  if (ImGui::RadioButton("Perspektivní projekce",
                         m_cameraSettings.projectionType == 2))
    m_cameraSettings.projectionType = 2;
  if (ImGui::RadioButton("Ortogonální projekce",
                         m_cameraSettings.projectionType == 3))
    m_cameraSettings.projectionType = 3;
  ImGui::SliderFloat("FOV", &m_cameraSettings.fov, 30.0f, 150.0f);
  if (m_cameraSettings.cameraType == 1) {
    ImGui::EndDisabled();
  }

  ImGui::Separator();
}
void WindowSystem::ApplyGuiData() {
  if (m_cameraSettings.projectionType == 2) {
    CameraSystem::GetInstance().SetProjMatToPerspective(s_viewportWinSize);
  } else if (m_cameraSettings.projectionType == 3) {
    CameraSystem::GetInstance().SetProjMatToOrtho();
  }

  if (m_cameraSettings.cameraType == 0) {
    CameraSystem::GetInstance().SetActiveCamera(Cameras::STATIC);
  } else if (m_cameraSettings.cameraType == 1) {
    CameraSystem::GetInstance().SetActiveCamera(Cameras::FIRST_PERSON);
  }

  CameraSystem::GetInstance().SetFov(m_cameraSettings.fov, s_viewportWinSize);
}

const glm::vec2 WindowSystem::GetViewportWinSize() { return s_viewportWinSize; }

InputData WindowSystem::GetInputData() { return m_inputData; }
