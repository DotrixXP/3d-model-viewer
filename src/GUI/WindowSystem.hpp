#pragma once
#include "../Core/Core.hpp"
#include "imgui.h"
#include <cstdint>
#include "../Tools/FileDialog.hpp"

struct InputData {
  glm::vec3 modelPos;
  glm::vec3 modelRot;
  glm::vec3 modelScale;
  float modelRotAngle;

  InputData(glm::vec3 modelPos, glm::vec3 modelRot, glm::vec3 modelScale,
            float modelRotAngle);
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
private:
  static glm::vec2 s_viewportWinSize;  
  inline void RenderPositionsWidgets();
  inline void RenderClearColorPicker();
  inline void RenderModelInfo();
  inline void RenderCameraSettings();
  inline ImVec2 RenderMainMenuBar();
  InputData m_inputData;
  CameraSettings m_cameraSettings;

};
