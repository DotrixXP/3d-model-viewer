#include "WindowSystem.hpp"
#include "imgui.h"

InputData::InputData(glm::vec3 modelPos, glm::vec3 modelRot,
                     glm::vec3 modelScale, float modelRotAngle)
    : modelPos(modelPos), modelRot(modelRot), modelScale(modelScale),
      modelRotAngle(modelRotAngle) {}
WindowSystem::WindowSystem()
    : m_inputData(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), 0.0f) {
  m_viewportWinSize = glm::vec2(uint32_t(500), uint32_t(500));
}
void WindowSystem::RenderWindows() {
  ImVec2 mainMenuBarSize;
  if (ImGui::BeginMainMenuBar()) {
    mainMenuBarSize = ImGui::GetWindowSize();
    if (ImGui::BeginMenu("Soubor")) {
      if (ImGui::MenuItem("Otevrit",
                          "Ctrl+O")) { // TODO: Funkce pro otevreni souboru
      }
      if (ImGui::MenuItem("Zavrit",
                          "Ctrl+W")) { // TODO: Funkce pro zavreni aplikace
        Window::DestroyWindow();       // TODO: Hází výjímku - vyřešit
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

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
    m_viewportWinSize = glm::vec2(winSize.x, winSize.y);
  ImGui::Image((void *)(intptr_t)Core::GetRenderTargetTexture(), winSize);
  ImGui::End();

  ImGui::Begin("Vlastnosti", nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove);
  RenderPositionsWidgets();
  RenderClearColorPicker();
  ImGui::Text("Zde budou vlastnosti modelu");

  ImGui::SetWindowSize(ImVec2(screenSize.x - screenSize.x / 1.5,
                              screenSize.y - mainMenuBarSize.y));
  ImGui::SetWindowPos(ImVec2(screenSize.x / 1.5, mainMenuBarSize.y));
  ImGui::End();
}

void WindowSystem::RenderPositionsWidgets() {
  ImGui::SliderFloat3("Pozice", &m_inputData.modelPos.x, -10.0f, 10.0f);
  ImGui::SliderFloat3("Rotace", &m_inputData.modelRot.x, -1.0f, 1.0f);
  ImGui::SliderFloat3("Velikost", &m_inputData.modelScale.x, 0.0f, 10.0f);
  ImGui::SliderFloat("Uhel rotace", &m_inputData.modelRotAngle, 0.0f, 360.0f);
}

void WindowSystem::RenderClearColorPicker() {
  static ImVec4 color = ImVec4(1.f, 1.f, 1.f, 1.f);
  ImGui::ColorEdit3("Clear color", (float *)&color);
  Core::SetBackgroundColor(glm::vec3(color.x, color.y, color.z));
}

glm::vec2 WindowSystem::GetViewportWinSize() { return m_viewportWinSize; }

InputData WindowSystem::GetInputData() { return m_inputData; }
