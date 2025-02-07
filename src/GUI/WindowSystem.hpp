#pragma once
#include <cstdint>
#include <filesystem>
#include <optional>
#include <variant>
#include <vector>
#include "../Texture/Material.hpp"
#include "../Core/Core.hpp"
#include "../Core/Window.hpp"
#include "../Tools/FileDialog.hpp"
#include "ExportWindow.hpp"
#include "GuiData.hpp"
#include "ImGuizmo.h"
#include "imgui.h"
#include "../constants.hpp"
#include "../Locale/Locale.hpp"

class WindowSystem {
  public:
    WindowSystem();
    void RenderWindows(bool isObjectRendered);
    InputData GetInputData();
    void ApplyGuiData();
    static const glm::vec2 GetViewportWinSize();

    static std::optional<std::filesystem::path> s_newModelPath;

    static bool s_flipTexture;

    static bool RenderMaterialDialog(MaterialSelection& materialSelection,
                                     std::vector<std::string>& textures);

    [[nodiscard]] std::optional<ExportData> GetExportData();

  private:
    ExportWindow m_exportWindow;

    static bool s_isDiffuseTexture;
    static glm::vec2 s_viewportWinSize;
    inline void RenderApplicationSettings();
    inline void RenderPositionsWidgets();
    inline void RenderRendererWidgets();
    inline void RenderClearColorPicker();
    inline void RenderModelInfo();
    inline void RenderCameraSettings();
    inline void RenderModelErrorWindow();
    inline void RenderTextureErrorWindow();
    inline void RenderShaderSettings();
    inline ImVec2 RenderMainMenuBar(bool isObjectRendered);
    inline void OpenModelSelectionDialog();
    inline void RenderGizmoSettings();
    InputData m_inputData;
    CameraSettings m_cameraSettings;
    ImGuizmo::OPERATION m_gizmoOperation;
    bool m_renderGizmo;
    float m_gizmoSizeMultiplier;
    static bool s_showModelErrorWindow;
    static bool s_showApplicationSettings;
    static bool s_showTextureErrorWindow;
};
