#include "PerfData.hpp"

float PerfData::s_currentFrame = 0;
float PerfData::s_fps = 0;
float PerfData::s_deltaTime = 0;
float PerfData::s_lastFrame = 0;

float PerfData::s_frameCount = 0;
float PerfData::s_previousTime = 0;

void PerfData::CollectPerformanceData()
{
    s_currentFrame = glfwGetTime();
    s_deltaTime = s_currentFrame - s_lastFrame;
    s_lastFrame = s_currentFrame;

    int framesBuffer = -1;
    s_frameCount++;

    if (s_currentFrame - s_previousTime >= 1.0)
    {
        framesBuffer = s_frameCount;
        s_frameCount = 0;
        s_previousTime = s_currentFrame;

        s_fps = framesBuffer;
    }
}

float PerfData::GetFPS() { return s_fps; }

float PerfData::GetDeltaTime() { return s_deltaTime; }
