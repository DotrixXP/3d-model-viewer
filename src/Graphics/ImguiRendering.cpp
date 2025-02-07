#include "ImguiRendering.hpp"
#include "../AssetManager.hpp"
#include "../AppSettings.hpp"

int32_t ImguiRendering::s_fontSize = 0;
std::map<int32_t, ImFont*> ImguiRendering::s_fonts;

void ImguiRendering::InitImgui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    const std::string fontPath = AssetManager::GetAssetPath(Assets::UI_FONT).string();
    std::cout << "Font path: " << fontPath << '\n';

    ImGuiIO& io = ImGui::GetIO();
    ImVector<ImWchar> ranges;
    ImFontGlyphRangesBuilder builder;
    builder.AddText("áčďéěíňóřšťúůýžÁČĎÉĚÍŇÓŘŠŤÚŮÝŽ");
    builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
    builder.BuildRanges(&ranges);

    for (int size = 18; size <= 30; ++size) {
        ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), size, nullptr, ranges.Data);
        if (font == nullptr) {
            Log::Error("Failed to load font: " + std::string(fontPath) + " with size "
                       + std::to_string(size));
        }
        else {
            s_fonts[size] = font;
        }
    }

    io.Fonts->Build();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    s_fontSize = SettingsManager::Get().GetLoadedSettings().fontSize;
    if (s_fonts.find(s_fontSize) != s_fonts.end()) {
        io.FontDefault = s_fonts[s_fontSize];
    }
}

void ImguiRendering::ChangeFontSize(float newSize) {
    int size = static_cast<int>(newSize);
    if (s_fonts.find(size) == s_fonts.end()) {
        Log::Error("Font size " + std::to_string(size) + " not loaded.");
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.FontDefault = s_fonts[size];
    s_fontSize = size;

    SettingsManager::Get().SetFontSize(size);
}

void ImguiRendering::OnRenderEnd() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiRendering::ShutdownImguiRendering() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImguiRendering::UpdateImgui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
