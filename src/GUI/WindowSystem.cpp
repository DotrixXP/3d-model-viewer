#include "WindowSystem.hpp"
#include "../Graphics/Renderer.hpp"
#include "ImGuizmo.h"
#include "imgui.h"

glm::vec2 WindowSystem::s_viewportWinSize = glm::vec2(-1.0f);
std::optional<std::filesystem::path> WindowSystem::s_modelPath = std::nullopt;

InputData::InputData(glm::vec3 modelPos, glm::vec3 modelRot,
                     glm::vec3 modelScale, float modelRotAngle)
    : modelPos(modelPos), modelRot(modelRot), modelScale(modelScale),
      modelRotAngle(modelRotAngle) {
  model = glm::mat4(1.0f);
  model = glm::translate(model, modelPos);
  model = glm::rotate(model, glm::radians(modelRotAngle), modelRot);
  model = glm::scale(model, modelScale);
}

const glm::mat4 InputData::GetModelMatrix() const {
  glm::mat4 scaleMatrix =
      glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));

  return scaleMatrix * model;
}

CameraSettings::CameraSettings() noexcept
    : cameraType(0), projectionType(2) {}

WindowSystem::WindowSystem()
    : m_inputData(glm::vec3(2.f, 0.f, -5.f), glm::vec3(1.0f), glm::vec3(1.0f),
                  0.0f),
      m_renderGizmo(false), m_gizmoOperation(ImGuizmo::TRANSLATE),
      m_gizmoSizeMultiplier(1.0f) {

  s_viewportWinSize = glm::vec2(uint32_t(500), uint32_t(500));
}

void WindowSystem::RenderWindows(bool isObjectRendered) {
  ImVec2 mainMenuBarSize = this->RenderMainMenuBar();

  ImVec2 screenSize = ImGui::GetIO().DisplaySize;

  ImGui::Begin("Viewport", nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove);
  ImGui::SetWindowSize(
      ImVec2(screenSize.x / 1.5, screenSize.y - mainMenuBarSize.y));
  ImGui::SetWindowPos(ImVec2(0, mainMenuBarSize.y));

  auto winSize = ImGui::GetWindowSize(); // Velikost okna viewportu (ImGui okna)

  winSize.y -= 40;
  if (winSize.x > 0 && winSize.y > 0)
    s_viewportWinSize = glm::vec2((uint16_t)winSize.x, (uint16_t)winSize.y);

  if (isObjectRendered)
    ImGui::Image((void *)(intptr_t)Core::GetRenderTargetTexture(),
                 ImVec2(s_viewportWinSize.x, s_viewportWinSize.y));
  else {
    ImGui::Text(u8"Není načtený žádný model");
    ImGui::Text(u8"Otevřete model pomocí menu Soubor -> Otevřít");
  }

  // Gizmo
  if (isObjectRendered && m_cameraSettings.cameraType != 1 && m_renderGizmo) {
    ImGuizmo::BeginFrame();
    ImGuizmo::Enable(true);
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetGizmoSizeClipSpace(0.15f * m_gizmoSizeMultiplier);
    ImGuizmo::SetRect(0, 0, s_viewportWinSize.x, s_viewportWinSize.y);
    ImGuizmo::Manipulate(
        glm::value_ptr(CameraSystem::GetInstance().GetViewMatrix()),
        glm::value_ptr(CameraSystem::GetInstance().GetProjectionMatrix()),
        m_gizmoOperation, ImGuizmo::WORLD, glm::value_ptr(m_inputData.model));
  }
  // --------------

  ImGui::End();

  ImGui::Begin("Vlastnosti", nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove);

  ImGui::SetWindowSize(ImVec2(screenSize.x - screenSize.x / 1.5,
                              screenSize.y - mainMenuBarSize.y));
  ImGui::SetWindowPos(ImVec2(screenSize.x / 1.5, mainMenuBarSize.y));

  if (!isObjectRendered)
    ImGui::BeginDisabled();

  RenderClearColorPicker();
  RenderModelInfo();
  RenderCameraSettings();
  RenderGizmoSettings();

  if (!isObjectRendered)
    ImGui::EndDisabled();

  ImGui::End();
}

void WindowSystem::OpenModelSelectionDialog() {
  s_modelPath = FileDialogManager::GetInstance().InvokeFileDialog();
}

ImVec2 WindowSystem::RenderMainMenuBar() {
  ImVec2 mainMenuBarSize = ImVec2(0, 0);

  if (ImGui::BeginMainMenuBar()) {
    mainMenuBarSize = ImGui::GetWindowSize();

    if (ImGui::BeginMenu("Soubor")) {
      if (ImGui::MenuItem(u8"Otevřít", "Ctrl+O")) {
        WindowSystem::OpenModelSelectionDialog();
      }

      if (ImGui::MenuItem(u8"Zavřít",
                          "Ctrl+W")) { // TODO: Funkce pro zavreni aplikace
        Window::DestroyWindow();       // TODO: Hází výjímku - vyřešit
      }
      ImGui::EndMenu();
    }

    if (m_cameraSettings.cameraType == 1) {
      ImGui::Separator();
      ImGui::Text(u8"Pohyb kamery pomocí WASD");
      ImGui::Text(u8"Pro odemčení kurzoru stiskněte F1, pro zamčení F2");
    }

    ImGui::EndMainMenuBar();
  }
  return mainMenuBarSize;
}

void WindowSystem::RenderClearColorPicker() {
  static ImVec4 color = ImVec4(1.f, 1.f, 1.f, 1.f);
  ImGui::ColorEdit3(u8"Barva pozadí", (float *)&color);
  Core::SetBackgroundColor(glm::vec3(color.x, color.y, color.z));
}

void WindowSystem::RenderModelInfo() {
  ImGui::Separator();
  ImGui::Text(u8"Model info");
  ImGui::Text(u8"Počet vertexů: %d", Renderer::GetInstance().GetVerticesCount());
  ImGui::Text(u8"Počet indexů: %d", Renderer::GetInstance().GetIndicesCount());
  ImGui::Separator();
}

void WindowSystem::RenderGizmoSettings() {
  ImGui::Text(u8"Nastavení gizma");
  ImGui::Checkbox(u8"Zobrazit gizmo", &m_renderGizmo);
  if (!m_renderGizmo) {
    ImGui::BeginDisabled();
  }

  ImGui::SliderFloat(u8"Velikost gizma", &m_gizmoSizeMultiplier, 0.5, 1.5);
  if (ImGui::RadioButton(u8"Přesun", m_gizmoOperation == 0))
    m_gizmoOperation = ImGuizmo::TRANSLATE;
  if (ImGui::RadioButton(u8"Rotace", m_gizmoOperation == 1))
    m_gizmoOperation = ImGuizmo::ROTATE;
  if (ImGui::RadioButton(u8"Škálování", m_gizmoOperation == 2))
    m_gizmoOperation = ImGuizmo::SCALE;

  if (!m_renderGizmo) {
    ImGui::EndDisabled();
  }

  ImGui::Separator();
}

void WindowSystem::RenderCameraSettings() {
  ImGui::Text(u8"Nastavení kamery");
  if (ImGui::RadioButton(u8"Freefly kamera", m_cameraSettings.cameraType == 1))
    m_cameraSettings.cameraType = 1;
  if (ImGui::RadioButton(u8"Orbitální kamera", m_cameraSettings.cameraType == 0))
    m_cameraSettings.cameraType = 0;
  if (m_cameraSettings.cameraType == 1) {
    ImGui::BeginDisabled();
  }
  ImGui::Separator();
  if (ImGui::RadioButton(u8"Perspektivní projekce",
                         m_cameraSettings.projectionType == 2))
    m_cameraSettings.projectionType = 2;
  if (ImGui::RadioButton(u8"Ortogonální projekce",
                         m_cameraSettings.projectionType == 3))
    m_cameraSettings.projectionType = 3;
  if (m_cameraSettings.cameraType == 1) {
    ImGui::EndDisabled();
  }

  ImGui::Separator();
}
void WindowSystem::ApplyGuiData() {
  
  CameraSystem::GetInstance().SetInputState(!m_renderGizmo);

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
}

const glm::vec2 WindowSystem::GetViewportWinSize() { return s_viewportWinSize; }

InputData WindowSystem::GetInputData() { return m_inputData; }
