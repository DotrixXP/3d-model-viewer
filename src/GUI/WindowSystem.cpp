#include "WindowSystem.hpp"
#include "../Model/ModelLoader.hpp"
#include "../Graphics/Renderer.hpp"
#include "ErrorWindow.hpp"
#include "ImGuizmo.h"
#include "imgui.h"
#include "GuiStyle.hpp"
#include "../AppSettings.hpp"
#include <ranges>
#include <utility>
#include <variant>

glm::vec2 WindowSystem::s_viewportWinSize = glm::vec2(-1.0f);
std::optional<std::filesystem::path> WindowSystem::s_newModelPath = std::nullopt;
bool WindowSystem::s_flipTexture = false;
bool WindowSystem::s_showTextureErrorWindow = false;
bool WindowSystem::s_showModelErrorWindow = false;
bool WindowSystem::s_showApplicationSettings = false;
bool WindowSystem::s_showPostProcessingSettings = false;
bool WindowSystem::s_isDiffuseTexture = true;

WindowSystem::WindowSystem()
    : m_inputData(glm::vec3(2.f, 0.f, -5.f), glm::vec3(1.0f), glm::vec3(1.0f), 0.0f, true)
    , m_gizmoOperation(ImGuizmo::TRANSLATE)
    , m_renderGizmo(false)
    , m_gizmoSizeMultiplier(1.0f) {
    SetGuiStyle();
    s_viewportWinSize = glm::vec2(uint32_t(500), uint32_t(500));
}

void WindowSystem::RenderApplicationSettings() {
    static Language s_currentLanguage
        = LanguageFromString(SettingsManager::Get().GetLoadedSettings().language);

    if (ImGui::Begin(Locale::GetText(LocaleKey::SETTINGS),
                     nullptr,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize)) {

        if (ImGui::BeginTabBar("SettingsTabs")) {

            if (ImGui::BeginTabItem(Locale::GetText(LocaleKey::LANGUAGE))) {
                ImGui::Text("%s", Locale::GetText(LocaleKey::CHOOSE_LANGUAGE));

                if (ImGui::RadioButton(Locale::GetText(LocaleKey::CZECH),
                                       s_currentLanguage == Language::CZECH)) {
                    s_currentLanguage = Language::CZECH;
                    Locale::SetLanguage(s_currentLanguage);
                }
                if (ImGui::RadioButton(Locale::GetText(LocaleKey::ENGLISH),
                                       s_currentLanguage == Language::ENGLISH)) {
                    s_currentLanguage = Language::ENGLISH;
                    Locale::SetLanguage(s_currentLanguage);
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem(Locale::GetText(LocaleKey::DISPLAY))) {
                ImGui::Text("%s", Locale::GetText(LocaleKey::FONT_SIZE));
                static int fontSize = SettingsManager::Get().GetLoadedSettings().fontSize;

                ImGui::SliderInt(Locale::GetText(LocaleKey::SET_FONT_SIZE), &fontSize, 18, 30);
                ImguiRendering::ChangeFontSize(fontSize);
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::Separator();
        float windowWidth = ImGui::GetWindowSize().x;
        float buttonWidth = ImGui::CalcTextSize(Locale::GetText(LocaleKey::CLOSE)).x
                            + ImGui::GetStyle().FramePadding.x * 2.0f;
        ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

        if (ImGui::Button(Locale::GetText(LocaleKey::CLOSE), ImVec2(buttonWidth, 0))) {
            s_showApplicationSettings = false;
        }

        ImGui::End();
    }
}

void WindowSystem::RenderWindows(bool isObjectRendered) {
    if (GuiErrorWindow::s_open)
        ImGui::BeginDisabled();

    ImVec2 mainMenuBarSize = this->RenderMainMenuBar(isObjectRendered);
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    ImGui::Begin(Locale::GetText(LocaleKey::VIEWPORT),
                 nullptr,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                     | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
                     | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::SetWindowSize(ImVec2(screenSize.x / 1.5, screenSize.y - mainMenuBarSize.y));
    ImGui::SetWindowPos(ImVec2(0, mainMenuBarSize.y));

    auto winSize = ImGui::GetWindowSize();

    winSize.y -= 40;
    if (winSize.x > 0 && winSize.y > 0)
        s_viewportWinSize = glm::vec2((uint16_t)winSize.x, (uint16_t)winSize.y);

    if (s_showPostProcessingSettings) {
        ImGui::Begin("Post processing options");
        ImGui::Text("Post processing options are applied after model reload!");
        RenderPostProcessingSettings();
        ImGui::End();
    }

    if (isObjectRendered)
        ImGui::Image(static_cast<ImTextureID>(Core::GetRenderTargetTexture()),
                     ImVec2(s_viewportWinSize.x, s_viewportWinSize.y));
    else {
        float windowWidth = ImGui::GetWindowSize().x;
        float windowHeight = ImGui::GetWindowSize().y;

        float textWidth = ImGui::CalcTextSize(Locale::GetText(LocaleKey::NO_MODEL_LOADED)).x;
        float textHeight = ImGui::CalcTextSize(Locale::GetText(LocaleKey::NO_MODEL_LOADED)).y;
        if (ImGui::BeginTabBar("ModelTabs")) {
            if (ImGui::BeginTabItem("Model loading")) {
                ImGui::SetCursorPos(ImVec2((windowWidth - textWidth) / 2.0f,
                                           (windowHeight - textHeight) / 2.0f - 60.f));
                ImGui::Text("%s", Locale::GetText(LocaleKey::NO_MODEL_LOADED));

                textWidth
                    = ImGui::CalcTextSize(Locale::GetText(LocaleKey::OPEN_MODEL_WITH_FILE_MENU)).x;
                textHeight
                    = ImGui::CalcTextSize(Locale::GetText(LocaleKey::OPEN_MODEL_WITH_FILE_MENU)).y;

                ImGui::SetCursorPos(ImVec2((windowWidth - textWidth) / 2.0f,
                                           (windowHeight - textHeight) / 2.0f - 30.f));
                ImGui::Text("%s", Locale::GetText(LocaleKey::OPEN_MODEL_WITH_FILE_MENU));

                ImGui::Dummy(ImVec2(0, 10));
                ImGui::Separator();
                ImGui::Dummy(ImVec2(0, 10));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(30, 10));
                ImGuiIO& io = ImGui::GetIO();
                float fontSize = io.FontDefault->FontSize;

                float buttonWidth = fontSize * 10.0f;
                float buttonHeight = fontSize * 3.0f;

                ImGui::SetCursorPosX((windowWidth - buttonWidth) / 2.0f);
                if (ImGui::Button(Locale::GetText(LocaleKey::LOAD_MODEL),
                                  ImVec2(buttonWidth, buttonHeight))) {
                    OpenModelSelectionDialog();
                }
                ImGui::PopStyleVar();

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Model post processing")) {
                RenderPostProcessingSettings();

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    this->m_inputData.m_allowCameraInput
        = (ImGui::IsWindowHovered() || !Window::GetCursorVisibility()) & !m_renderGizmo;

    // Gizmo
    if (isObjectRendered && m_cameraSettings.cameraType != 1 && m_renderGizmo) {
        ImGuizmo::BeginFrame();
        ImGuizmo::Enable(true);
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetGizmoSizeClipSpace(0.15f * m_gizmoSizeMultiplier);
        ImGuizmo::SetRect(0, 0, s_viewportWinSize.x, s_viewportWinSize.y);
        ImGuizmo::Manipulate(glm::value_ptr(CameraSystem::GetInstance().GetViewMatrix()),
                             glm::value_ptr(CameraSystem::GetInstance().GetProjectionMatrix()),
                             m_gizmoOperation,
                             ImGuizmo::WORLD,
                             glm::value_ptr(m_inputData.model));
    }
    // --------------

    ImGui::End();

    ImGui::Begin(Locale::GetText(LocaleKey::PROPERTIES),
                 nullptr,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                     | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::SetWindowSize(
        ImVec2(screenSize.x - screenSize.x / 1.5, screenSize.y - mainMenuBarSize.y));
    ImGui::SetWindowPos(ImVec2(screenSize.x / 1.5, mainMenuBarSize.y));

    if (!isObjectRendered)
        ImGui::BeginDisabled();

    RenderClearColorPicker();
    RenderRendererWidgets();
    RenderModelInfo();

    ImGui::Separator();
    if (m_renderGizmo)
        ImGui::BeginDisabled();
    RenderCameraSettings();
    if (m_renderGizmo)
        ImGui::EndDisabled();

    RenderGizmoSettings();
    RenderShaderSettings();

    if (!isObjectRendered)
        ImGui::EndDisabled();

    RenderModelErrorWindow();
    RenderTextureErrorWindow();
    ImGui::End();

    if (s_showApplicationSettings) {
        RenderApplicationSettings();
    }

    if (GuiErrorWindow::s_open)
        ImGui::EndDisabled();

    m_exportWindow.Render();

    GuiErrorWindow::Render();
}

void WindowSystem::RenderRendererWidgets() {
    if (ImGui::RadioButton(Locale::GetText(LocaleKey::FILL_MODE),
                           m_inputData.m_polygonMode == PolygonMode::FILL)) {
        m_inputData.m_polygonMode = PolygonMode::FILL;
    }
    if (ImGui::RadioButton(Locale::GetText(LocaleKey::WIREFRAME_MODE),
                           m_inputData.m_polygonMode == PolygonMode::LINE)) {
        m_inputData.m_polygonMode = PolygonMode::LINE;
    }
    if (ImGui::RadioButton(Locale::GetText(LocaleKey::POINTS_MODE),
                           m_inputData.m_polygonMode == PolygonMode::POINT)) {
        m_inputData.m_polygonMode = PolygonMode::POINT;
    }
}

void WindowSystem::OpenModelSelectionDialog() {
    auto path = FileDialogManager::GetInstance().InvokeFileDialog();

    std::filesystem::path modelPath = path;
    std::string extension = modelPath.extension().string();

    if (modelPath.empty()) {
        Log::Info(Locale::GetText(LocaleKey::FILE_WAS_NOT_SELECTED));
        return;
    }

    if (std::find(SUPPORTED_MODEL_EXTENSIONS.begin(), SUPPORTED_MODEL_EXTENSIONS.end(), extension)
        != SUPPORTED_MODEL_EXTENSIONS.end()) {
        s_newModelPath = path;
    }
    else {
        s_showModelErrorWindow = true;
        Log::Info("Not supported file extension: " + extension);
    }
}

void WindowSystem::RenderModelErrorWindow() {
    if (s_showModelErrorWindow) {
        ImGui::OpenPopup(Locale::GetText(LocaleKey::FILE_WAS_NOT_SELECTED));
        if (ImGui::BeginPopupModal(Locale::GetText(LocaleKey::MODEL_WAS_NOT_SELECTED),
                                   nullptr,
                                   ImGuiWindowFlags_AlwaysAutoResize
                                       | ImGuiWindowFlags_NoCollapse)) {
            ImGui::Text("%s", Locale::GetText(LocaleKey::FILE_EXTENSION_NOT_SUPPORTED));
            ImGui::Separator();

            if (ImGui::Button(Locale::GetText(LocaleKey::OK), ImVec2(120, 0))) {
                s_showModelErrorWindow = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}

void WindowSystem::RenderTextureErrorWindow() {
    if (s_showTextureErrorWindow) {
        ImGui::OpenPopup(Locale::GetText(LocaleKey::CHOOSED_TEXTURE_HAS_UNKNOWN_EXTENSION));
        if (ImGui::BeginPopupModal(
                Locale::GetText(LocaleKey::CHOOSED_TEXTURE_HAS_UNKNOWN_EXTENSION),
                nullptr,
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse)) {
            ImGui::Text("%s", Locale::GetText(LocaleKey::CHOOSED_TEXTURE_HAS_UNKNOWN_EXTENSION));
            ImGui::Separator();

            if (ImGui::Button(Locale::GetText(LocaleKey::OK), ImVec2(120, 0))) {
                s_showTextureErrorWindow = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}

void WindowSystem::RenderShaderSettings() {
    ImGui::Text("%s", Locale::GetText(LocaleKey::SHADER_SETTINGS));
    int shaderType = 0;
    if (m_inputData.m_simpleShaderActive)
        shaderType = 0;
    if (m_inputData.m_lightShaderActive)
        shaderType = 1;
    if (m_inputData.m_reflectShaderActive)
        shaderType = 2;

    if (ImGui::RadioButton(Locale::GetText(LocaleKey::SIMPLE_SHADER), shaderType == 0)) {
        shaderType = 0;
    }
    if (ImGui::RadioButton(Locale::GetText(LocaleKey::LIGHT_SHADER), shaderType == 1)) {
        shaderType = 1;
    }

    if (!m_inputData.m_skyboxActive)
        ImGui::BeginDisabled();

    if (ImGui::RadioButton(Locale::GetText(LocaleKey::REFLECT_SHADER), shaderType == 2)) {
        shaderType = 2;
    }

    if (!m_inputData.m_skyboxActive)
        ImGui::EndDisabled();

    m_inputData.m_simpleShaderActive = (shaderType == 0);
    m_inputData.m_lightShaderActive = (shaderType == 1);
    m_inputData.m_reflectShaderActive = (shaderType == 2);

    if (m_inputData.m_reflectShaderActive) {
        ImGui::BeginDisabled();
    }

    ImGui::Checkbox(Locale::GetText(LocaleKey::ENABLE_SKYBOX), &m_inputData.m_skyboxActive);

    if (m_inputData.m_reflectShaderActive) {
        ImGui::EndDisabled();
    }

    if (!m_inputData.m_lightShaderActive) {
        ImGui::BeginDisabled();
    }

    ImGui::SliderFloat(
        Locale::GetText(LocaleKey::LIGHT_INTENSITY), &m_inputData.m_lightIntensity, 0.0f, 1.0f);
    ImGui::SliderFloat(Locale::GetText(LocaleKey::MATERIAL_SHININESS),
                       &m_inputData.m_materialShininess,
                       1.0f,
                       256.0f);
    ImGui::DragFloat3(
        Locale::GetText(LocaleKey::LIGHT_POS), glm::value_ptr(m_inputData.m_lightPos), 1.0f);
    if (!m_inputData.m_lightShaderActive) {
        ImGui::EndDisabled();
    }

    ImGui::Separator();
}

ImVec2 WindowSystem::RenderMainMenuBar(bool isObjectRendered) {
    ImVec2 mainMenuBarSize = ImVec2(0, 0);

    if (ImGui::BeginMainMenuBar()) {
        mainMenuBarSize = ImGui::GetWindowSize();

        if (ImGui::BeginMenu(Locale::GetText(LocaleKey::FILE))) {
            if (ImGui::MenuItem(Locale::GetText(LocaleKey::OPEN))) {
                WindowSystem::OpenModelSelectionDialog();
            }

            if (ImGui::MenuItem(Locale::GetText(LocaleKey::CLOSE))) {
                exit(0);
            }
            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::Button(Locale::GetText(LocaleKey::SETTINGS))) {
            s_showApplicationSettings = !s_showApplicationSettings;
        }

        if (!isObjectRendered)
            ImGui::BeginDisabled();

        if (ImGui::Button(Locale::GetText(LocaleKey::EXPORT_MODEL))) {
            m_exportWindow.m_open = true;
        }

        if (!isObjectRendered)
            ImGui::EndDisabled();

        if (ImGui::Button("Post processing options")) {
            s_showPostProcessingSettings = !s_showPostProcessingSettings;
        }

        if (m_cameraSettings.cameraType == 1) {
            ImGui::Separator();
            ImGui::Text("%s", Locale::GetText(LocaleKey::MOVE_CAMERA_USING_WASD));
            ImGui::Text("%s",
                        Locale::GetText(LocaleKey::FOR_UNLOCK_CURSOR_PRESS_F2_FOR_LOCK_PRESS_F1));
        }

        ImGui::EndMainMenuBar();
    }
    return mainMenuBarSize;
}

void WindowSystem::RenderClearColorPicker() {
    static ImVec4 color = ImVec4(1.f, 1.f, 1.f, 1.f);
    ImGui::ColorEdit3(Locale::GetText(LocaleKey::BACKGROUND_COLOR), (float*)&color);
    Core::SetBackgroundColor(glm::vec3(color.x, color.y, color.z));
}

void WindowSystem::RenderModelInfo() {
    ImGui::Separator();
    ImGui::Text("%s", Locale::GetText(LocaleKey::MODEL_INFO));
    ImGui::Text((std::string(Locale::GetText(LocaleKey::VERTEX_COUNT)) + std::string(" ")
                 + std::string("%d"))
                    .c_str(),
                Renderer::GetInstance().GetVerticesCount());
    ImGui::Text((std::string(Locale::GetText(LocaleKey::INDEX_COUNT)) + std::string(" ")
                 + std::string("%d"))
                    .c_str(),
                Renderer::GetInstance().GetIndicesCount());
    ImGui::Separator();
}

void WindowSystem::RenderPostProcessingSettings() {
    ImGui::Separator();

    static std::uint32_t postProcessingOptions = 0;

    // Render checkboxes for each PostProcessing flag
    ImGui::CheckboxFlags("Calc Tangent Space",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::CALC_TANGENT_SPACE));
    ImGui::CheckboxFlags("Join Identical Vertices",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::JOIN_IDENTICAL_VERTICES));
    ImGui::CheckboxFlags("Make Left Handed",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::MAKE_LEFT_HANDED));
    ImGui::CheckboxFlags("Triangulate",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::TRIANGULATE));
    ImGui::CheckboxFlags("Remove Component",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::REMOVE_COMPONENT));
    ImGui::CheckboxFlags("Generate Normals",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::GEN_NORMALS));
    ImGui::CheckboxFlags("Generate Smooth Normals",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::GEN_SMOOTH_NORMALS));
    ImGui::CheckboxFlags("Split Large Meshes",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::SPLIT_LARGE_MESHES));
    ImGui::CheckboxFlags("Pre-transform Vertices",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::PRE_TRANSFORM_VERTICES));
    ImGui::CheckboxFlags("Limit Bone Weights",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::LIMIT_BONE_WEIGHTS));
    ImGui::CheckboxFlags("Validate Data Structure",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::VALIDATE_DATA_STRUCTURE));
    ImGui::CheckboxFlags("Improve Cache Locality",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::IMPROVE_CACHE_LOCALITY));
    ImGui::CheckboxFlags("Remove Redundant Materials",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::REMOVE_REDUNDANT_MATERIALS));
    ImGui::CheckboxFlags("Fix Infacing Normals",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::FIX_INFACING_NORMALS));
    ImGui::CheckboxFlags("Sort by pType",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::SORT_BY_PTYPE));
    ImGui::CheckboxFlags("Find Degenerates",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::FIND_DEGENERATES));
    ImGui::CheckboxFlags("Find Invalid Data",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::FIND_INVALID_DATA));
    ImGui::CheckboxFlags("Generate UV Coordinates",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::GEN_UV_COORDS));
    ImGui::CheckboxFlags("Transform UV Coordinates",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::TRANSFORM_UV_COORDS));
    ImGui::CheckboxFlags("Find Instances",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::FIND_INSTANCES));
    ImGui::CheckboxFlags("Optimize Meshes",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::OPTIMIZE_MESHES));
    ImGui::CheckboxFlags("Optimize Graph",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::OPTIMIZE_GRAPH));
    ImGui::CheckboxFlags(
        "Flip UVs", &postProcessingOptions, static_cast<std::uint32_t>(PostProcessing::FLIP_UVS));
    ImGui::CheckboxFlags("Flip Winding Order",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::FLIP_WINDING_ORDER));
    ImGui::CheckboxFlags("Split by Bone Count",
                         &postProcessingOptions,
                         static_cast<std::uint32_t>(PostProcessing::SPLIT_BY_BONE_COUNT));
    ImGui::CheckboxFlags(
        "Debone", &postProcessingOptions, static_cast<std::uint32_t>(PostProcessing::DEBONE));

    ModelLoader::SetPostProcessingOptions(postProcessingOptions);
}

bool WindowSystem::RenderMaterialDialog(MaterialSelection& materialSelection,
                                        std::vector<std::string>& textures) {
    if (s_showTextureErrorWindow) {
        return false;
    }

    ImVec2 windowSize(400, 300);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);

    ImGui::Begin(Locale::GetText(LocaleKey::AUTOMATIC_TEXTURE_SELECTION),
                 nullptr,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    const uint32_t textureListHeight = 5;

    static ImVec4 color = ImVec4(1.f, 1.f, 1.f, 1.f);
    if (ImGui::BeginTabBar("MaterialTabs")) {
        if (ImGui::BeginTabItem(Locale::GetText(LocaleKey::DIFFUSE))) {
            ImGui::Text("%s", Locale::GetText(LocaleKey::CHOOSE_DIFFUSE_TEXTURE));

            if (ImGui::RadioButton(Locale::GetText(LocaleKey::DIFFUSE_COLOR),
                                   s_isDiffuseTexture == 0)) {
                s_isDiffuseTexture = false;
            }

            if (ImGui::RadioButton(Locale::GetText(LocaleKey::DIFFUSE_TEXTURE),
                                   s_isDiffuseTexture == 1)) {
                s_isDiffuseTexture = true;
            }

            if (s_isDiffuseTexture == 0)
                ImGui::BeginDisabled();

            ImGui::BeginChild("DiffuseTextures",
                              ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * textureListHeight),
                              true);
            for (auto& texture : textures) {
                bool isSelected
                    = materialSelection.diffuse.has_value()
                      && std::holds_alternative<std::string>(materialSelection.diffuse.value())
                      && std::get<std::string>(materialSelection.diffuse.value()) == texture;

                if (ImGui::Selectable(
                        (std::filesystem::path(texture).filename().string() + " -> Diffuse?")
                            .c_str(),
                        isSelected)) {
                    materialSelection.diffuse = texture;
                    Log::Info("Diffuse texture: " + texture);
                }

                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("%s", texture.c_str());
                    ImGui::EndTooltip();
                }

                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndChild();

            ImGui::Separator();
            ImGui::Checkbox(Locale::GetText(LocaleKey::FLIP_TEXTURE), &s_flipTexture);
            if (ImGui::Button(Locale::GetText(LocaleKey::CHOOSE_TEXTURE_MANUALLY))) {
                auto texture = FileDialogManager::GetInstance().InvokeFileDialog();
                if (texture.empty()) {
                    ImGui::EndTabItem();
                    ImGui::EndTabBar();
                    ImGui::End();
                    return false;
                }

                std::string extension = std::filesystem::path(texture).extension().string();
                if (std::find(SUPPORTED_TEXTURE_EXTENSIONS.begin(),
                              SUPPORTED_TEXTURE_EXTENSIONS.end(),
                              extension)
                    == SUPPORTED_TEXTURE_EXTENSIONS.end()) {
                    Log::Error("Unsuported texture format" + texture);
                    s_showTextureErrorWindow = true;
                    ImGui::EndTabBar();
                    ImGui::End();
                    return false;
                }
                else {
                    materialSelection.diffuse = texture;
                }
            }

            if (s_isDiffuseTexture == 0)
                ImGui::EndDisabled();

            if (s_isDiffuseTexture == 1)
                ImGui::BeginDisabled();
            ImGui::Separator();
            ImGui::Text("%s", Locale::GetText(LocaleKey::CHOOSE_COLOR_MANUALLY));

            ImGui::Text("%s", Locale::GetText(LocaleKey::CHOOSE_MODEL_COLOR));
            ImGui::ColorEdit3(Locale::GetText(LocaleKey::MODEL_COLOR), (float*)&color);

            if (s_isDiffuseTexture == 1)
                ImGui::EndDisabled();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(Locale::GetText(LocaleKey::SPECULAR))) {
            ImGui::Text("%s", Locale::GetText(LocaleKey::CHOOSE_SPECULAR_TEXTURE));

            ImGui::BeginChild("SpecularTextures",
                              ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * textureListHeight),
                              true);
            for (auto& texture : textures) {
                bool isSelected = (materialSelection.specular == texture);
                if (ImGui::Selectable(
                        (std::filesystem::path(texture).filename().string() + " -> Specular?")
                            .c_str(),
                        isSelected)) {
                    materialSelection.specular = texture;
                    Log::Info("Specular texture: " + texture);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }

                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("%s", texture.c_str());
                    ImGui::EndTooltip();
                }
            }

            ImGui::EndChild();
            ImGui::Separator();
            ImGui::Checkbox(Locale::GetText(LocaleKey::FLIP_TEXTURE), &s_flipTexture);
            if (ImGui::Button(Locale::GetText(LocaleKey::CHOOSE_TEXTURE_MANUALLY))) {
                auto texture = FileDialogManager::GetInstance().InvokeFileDialog();
                std::string extension = std::filesystem::path(texture).extension().string();
                if (std::find(SUPPORTED_TEXTURE_EXTENSIONS.begin(),
                              SUPPORTED_TEXTURE_EXTENSIONS.end(),
                              extension)
                    == SUPPORTED_TEXTURE_EXTENSIONS.end()) {
                    Log::Error("Unsuported texture format:" + texture);
                    s_showTextureErrorWindow = true;
                    ImGui::EndTabBar();
                    ImGui::End();
                    return false;
                }
                else {
                    materialSelection.specular = texture;
                }
            }
            ImGui::EndTabItem();
        }

        // Normal map is not localed cuz its easier to understand in english
        if (ImGui::BeginTabItem("Normal map")) {
            ImGui::Text("%s", Locale::GetText(LocaleKey::CHOOSE_SPECULAR_TEXTURE));

            ImGui::BeginChild("NormalTextures",
                              ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * textureListHeight),
                              true);
            for (auto& texture : textures) {
                bool isSelected = (materialSelection.normal == texture);
                if (ImGui::Selectable(
                        (std::filesystem::path(texture).filename().string() + " -> Normal?")
                            .c_str(),
                        isSelected)) {
                    materialSelection.normal = texture;
                    Log::Info("Specular texture: " + texture);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }

                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("%s", texture.c_str());
                    ImGui::EndTooltip();
                }
            }

            ImGui::EndChild();
            ImGui::Separator();
            ImGui::Checkbox(Locale::GetText(LocaleKey::FLIP_TEXTURE), &s_flipTexture);
            if (ImGui::Button(Locale::GetText(LocaleKey::CHOOSE_TEXTURE_MANUALLY))) {
                auto texture = FileDialogManager::GetInstance().InvokeFileDialog();
                std::string extension = std::filesystem::path(texture).extension().string();
                if (std::find(SUPPORTED_TEXTURE_EXTENSIONS.begin(),
                              SUPPORTED_TEXTURE_EXTENSIONS.end(),
                              extension)
                    == SUPPORTED_TEXTURE_EXTENSIONS.end()) {
                    Log::Error("Unsuported texture format:" + texture);
                    s_showTextureErrorWindow = true;
                    ImGui::EndTabBar();
                    ImGui::End();
                    return false;
                }
                else {
                    materialSelection.normal = texture;
                }
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Separator();
    if (ImGui::Button(Locale::GetText(LocaleKey::APPLY))) {
        ImGui::End();
        if (s_isDiffuseTexture == 0) {
            materialSelection.diffuse = glm::vec3(color.x, color.y, color.z);
        }
        else if (s_isDiffuseTexture == 1 && !materialSelection.diffuse.has_value()) {
            return false;
        }
        return true;
    }
    ImGui::End();

    return false;
}

std::optional<ExportData> WindowSystem::GetExportData() {
    return m_exportWindow.GetExportData();
}

void WindowSystem::RenderGizmoSettings() {
    ImGui::Text("%s", Locale::GetText(LocaleKey::GIZMO_SETTINGS));
    ImGui::Checkbox(Locale::GetText(LocaleKey::SHOW_GIZMO), &m_renderGizmo);
    if (!m_renderGizmo) {
        ImGui::BeginDisabled();
    }

    ImGui::SliderFloat(Locale::GetText(LocaleKey::GIZMO_SIZE), &m_gizmoSizeMultiplier, 0.5, 1.5);
    if (ImGui::RadioButton(Locale::GetText(LocaleKey::TRANSLATE),
                           m_gizmoOperation == ImGuizmo::TRANSLATE))
        m_gizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::RadioButton(Locale::GetText(LocaleKey::ROTATE),
                           m_gizmoOperation == ImGuizmo::ROTATE))
        m_gizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::RadioButton(Locale::GetText(LocaleKey::SCALE), m_gizmoOperation == ImGuizmo::SCALE))
        m_gizmoOperation = ImGuizmo::SCALE;

    if (!m_renderGizmo) {
        ImGui::EndDisabled();
    }

    ImGui::Separator();
}

void WindowSystem::RenderCameraSettings() {
    ImGui::Text("%s", Locale::GetText(LocaleKey::CAMERA_SETTINGS));
    if (ImGui::RadioButton(Locale::GetText(LocaleKey::FREEFLY_CAMERA),
                           m_cameraSettings.cameraType == 1))
        m_cameraSettings.cameraType = 1;
    if (ImGui::RadioButton(Locale::GetText(LocaleKey::ORBIT_CAMERA),
                           m_cameraSettings.cameraType == 0))
        m_cameraSettings.cameraType = 0;
    if (m_cameraSettings.cameraType == 1) {
        ImGui::BeginDisabled();
    }
    ImGui::Separator();
    if (ImGui::RadioButton(Locale::GetText(LocaleKey::PERSPECTIVE_PROJECTION),
                           m_cameraSettings.projectionType == 2))
        m_cameraSettings.projectionType = 2;
    if (ImGui::RadioButton(Locale::GetText(LocaleKey::ORTHOGRAPHIC_PROJECTION),
                           m_cameraSettings.projectionType == 3))
        m_cameraSettings.projectionType = 3;

    if (m_cameraSettings.projectionType != 2)
        ImGui::BeginDisabled();
    ImGui::SliderFloat(
        Locale::GetText(LocaleKey::ZOOM), &m_cameraSettings.zoomMultiplier, 0.1f, 10.0f);
    if (m_cameraSettings.projectionType != 2)
        ImGui::EndDisabled();

    if (m_cameraSettings.cameraType == 1) {
        ImGui::EndDisabled();
    }

    ImGui::Separator();
}

void WindowSystem::ApplyGuiData() {
    if (m_cameraSettings.projectionType == 2) {
        CameraSystem::GetInstance().SetProjMatToPerspective();
    }
    else if (m_cameraSettings.projectionType == 3) {
        CameraSystem::GetInstance().SetProjMatToOrtho();
    }

    if (m_cameraSettings.cameraType == 0) {
        CameraSystem::GetInstance().SetActiveCamera(Cameras::ARCBALL);
    }
    else if (m_cameraSettings.cameraType == 1) {
        CameraSystem::GetInstance().SetActiveCamera(Cameras::FIRST_PERSON);
    }

    CameraSystem::GetInstance().SetZoomMultiplier(m_cameraSettings.zoomMultiplier);
}

const glm::vec2 WindowSystem::GetViewportWinSize() {
    return s_viewportWinSize;
}

InputData WindowSystem::GetInputData() {
    return m_inputData;
}
