#include "ErrorWindow.hpp"
#include <imgui.h>
#include "../Locale/Locale.hpp"

std::string GuiErrorWindow::s_errorMessage = "";
bool GuiErrorWindow::s_open = false;

void GuiErrorWindow::RaiseError(const std::string& errorMessage) {
    s_errorMessage = errorMessage;
    s_open = true;
}

void GuiErrorWindow::Render() {
    if (s_open) {
        // Get the IO object to access display size
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

        // Set the next window position and size
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // Begin the window with the desired flags
        ImGui::Begin("Error",
                     &s_open,
                     ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse
                         | ImGuiWindowFlags_NoTitleBar);

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
        float textWidth = ImGui::CalcTextSize(Locale::GetText(LocaleKey::UNEXPECTED_ERROR)).x;
        float availableWidth = ImGui::GetContentRegionAvail().x;
        ImGui::SetCursorPosX((availableWidth - textWidth) * 0.5f);
        ImGui::Text("%s", Locale::GetText(LocaleKey::UNEXPECTED_ERROR));

        ImGui::PopStyleColor(); // Restore original text color
        ImGui::Separator();

        ImGui::Text(
            "%s",
            (Locale::GetText(LocaleKey::MESSAGE) + std::string(": ") + s_errorMessage).c_str());

        ImGui::Spacing();
        ImGui::Spacing();

        // Calculate the button position to center it
        float buttonWidth = 100.0f;
        ImGui::SetCursorPosX((availableWidth - buttonWidth) * 0.5f);
        if (ImGui::Button(Locale::GetText(LocaleKey::CLOSE), ImVec2(buttonWidth, 0))) {
            s_open = false;
        }

        // End the window
        ImGui::End();
    }
}
