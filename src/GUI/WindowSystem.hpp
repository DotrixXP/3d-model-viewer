#pragma once
#include "../Core/Core.hpp"
#include "../Core/Window.hpp"
#include "../Tools/FileDialog.hpp"
#include "ImGuizmo.h"
#include "imgui.h"
#include <cstdint>
#include <filesystem>
#include <optional>
#include <vector>
#include "../constants.hpp"

struct InputData
{
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

struct CameraSettings
{
    int cameraType;
    int projectionType;
    CameraSettings() noexcept;
};

class WindowSystem
{
  public:
    WindowSystem();
    void RenderWindows(bool isObjectRendered);
    InputData GetInputData();
    void ApplyGuiData();
    static const glm::vec2 GetViewportWinSize();
    static std::optional<std::filesystem::path> s_modelPath;
    static const std::optional<std::string>
    RenderTexturesDialog(std::vector<std::string> textures);
    static const std::optional<glm::vec3> RenderModelColorPicker();

  private:
    static glm::vec2 s_viewportWinSize;
    inline void RenderPositionsWidgets();
    inline void RenderClearColorPicker();
    inline void RenderModelInfo();
    inline void RenderCameraSettings();
    inline void RenderModelErrorWindow();
    inline ImVec2 RenderMainMenuBar();
    inline void OpenModelSelectionDialog();
    inline void RenderGizmoSettings();
    InputData m_inputData;
    CameraSettings m_cameraSettings;
    ImGuizmo::OPERATION m_gizmoOperation;
    bool m_renderGizmo;
    float m_gizmoSizeMultiplier;
    bool m_showModelErrorWindow = false;
};
