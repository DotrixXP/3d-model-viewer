#pragma once
#include "../Core/Core.hpp"
#include "../Tools/FileDialog.hpp"
#include "ImGuizmo.h"
#include "imgui.h"
#include <cstdint>
#include <filesystem>
#include <optional>

struct InputData {
private:
  glm::vec3 modelPos;
  glm::vec3 modelRot;
  glm::vec3 modelScale;
  float modelRotAngle;

protected:
  glm::mat4 model;

public:
  InputData(glm::vec3 modelPos, glm::vec3 modelRot, glm::vec3 modelScale,
            float modelRotAngle);
  const glm::mat4 GetModelMatrix() const;

  friend class WindowSystem;
};

struct CameraSettings {
  int cameraType;
  int projectionType;
  float fov;
  CameraSettings() noexcept;
};

class WindowSystem {
public:
  WindowSystem();
  void RenderWindows();
  InputData GetInputData();
  void ApplyGuiData();
  static const glm::vec2 GetViewportWinSize();
  static std::optional<std::filesystem::path> s_modelPath;

private:
  static glm::vec2 s_viewportWinSize;
  inline void RenderPositionsWidgets();
  inline void RenderClearColorPicker();
  inline void RenderModelInfo();
  inline void RenderCameraSettings();
  inline ImVec2 RenderMainMenuBar();
  inline void RenderGizmo();
  inline void OpenModelSelectionDialog();
  InputData m_inputData;
  CameraSettings m_cameraSettings;
};
