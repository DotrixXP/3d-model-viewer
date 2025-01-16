#include "ErrorWindow.hpp"
#include <imgui.h>

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
        ImGui::SetNextWindowSize(ImVec2(230, 180)); // Set the window size to be bigger and constant

        // Begin the window with the desired flags
        ImGui::Begin("Error", &s_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

        // Get the window size
        ImVec2 windowSize = ImGui::GetWindowSize();

        // Calculate the text position to center it
        ImVec2 textSize = ImGui::CalcTextSize(s_errorMessage.c_str());
        ImVec2 textPos
            = ImVec2((windowSize.x - textSize.x) * 0.5f, (windowSize.y - textSize.y) * 0.4f);
        ImGui::SetCursorPos(textPos);
        ImGui::TextWrapped("%s", s_errorMessage.c_str());

        ImGui::Spacing();
        ImGui::Spacing();

        // Calculate the button position to center it
        float buttonWidth = 100.0f;
        float buttonHeight = ImGui::GetFrameHeight();
        ImVec2 buttonPos
            = ImVec2((windowSize.x - buttonWidth) * 0.5f, (windowSize.y - buttonHeight) * 0.6f);
        ImGui::SetCursorPos(buttonPos);
        if (ImGui::Button("OK", ImVec2(buttonWidth, 0))) {
            s_open = false;
        }

        // End the window
        ImGui::End();
    }
}
