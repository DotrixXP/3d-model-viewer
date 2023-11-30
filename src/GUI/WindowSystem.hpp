#pragma once
#include "../Core/Core.hpp"
#include "imgui.h"
#include <cstdint>

struct InputData {
  glm::vec3 modelPos;
  glm::vec3 modelRot;
  glm::vec3 modelScale;
  float modelRotAngle;

  InputData(glm::vec3 modelPos, glm::vec3 modelRot, glm::vec3 modelScale,
            float modelRotAngle);
};

class WindowSystem {
public:
  WindowSystem();
  void RenderWindows();
  InputData GetInputData();
  glm::vec2 GetViewportWinSize();

private:
  inline void RenderPositionsWidgets();
  inline void RenderClearColorPicker();
  glm::vec2 m_viewportWinSize;
  InputData m_inputData;
};
