#include "WindowSystem.hpp"
#include "imgui.h"

void WindowSystem::RenderWindows() {
  ImVec2 mainMenuBarSize;
  if (ImGui::BeginMainMenuBar()) {
    mainMenuBarSize = ImGui::GetWindowSize();
    if (ImGui::BeginMenu("Soubor")) {
      if (ImGui::MenuItem("Otevrit", "Ctrl+O")) { // TODO: Funkce pro otevreni souboru
      }
      if (ImGui::MenuItem("Zavrit", "Ctrl+W")) { // TODO: Funkce pro zavreni aplikace
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
   }

  ImVec2 screenSize = ImGui::GetIO().DisplaySize;

  ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
               | ImGuiWindowFlags_NoMove);
  ImGui::SetWindowSize(ImVec2(screenSize.x / 1.5, screenSize.y-mainMenuBarSize.y));
  ImGui::SetWindowPos(ImVec2(0, mainMenuBarSize.y));
  auto winSize = ImGui::GetWindowSize();
  winSize.y -= 40;
  ImGui::Image((void *)(intptr_t)Core::GetRenderTargetTexture(), winSize);
  ImGui::End();

  ImGui::Begin("Vlastnosti", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
               | ImGuiWindowFlags_NoMove);
  ImGui::Text("Zde budou vlastnosti modelu");

  ImGui::SetWindowSize(ImVec2(screenSize.x - screenSize.x / 1.5, screenSize.y-mainMenuBarSize.y));
  ImGui::SetWindowPos(ImVec2(screenSize.x / 1.5, mainMenuBarSize.y));
  ImGui::End();
}
