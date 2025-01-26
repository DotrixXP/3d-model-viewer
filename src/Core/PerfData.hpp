#pragma once
#include "../Core/Window.hpp"
#include "../Types.hpp"

class PerfData {
  private:
    static float64 s_fps;
    static float64 s_lastFrame;
    static float64 s_currentFrame;
    static float64 s_deltaTime;

    static float64 s_frameCount;
    static float64 s_previousTime;

  public:
    static void CollectPerformanceData();
    static float64 GetFPS();
    static float64 GetDeltaTime();
};
