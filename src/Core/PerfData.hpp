#pragma once
#include "../Core/Window.hpp"

class PerfData {
private:
  static float s_fps;
  static float s_lastFrame;
  static float s_currentFrame;
  static float s_deltaTime;

  static float s_frameCount;
  static float s_previousTime;

public:
  static void CollectPerformanceData();
  static float GetFPS();
  static float GetDeltaTime();
};
