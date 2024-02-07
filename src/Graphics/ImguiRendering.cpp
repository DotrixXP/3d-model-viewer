#include "ImguiRendering.hpp"

void ImguiRendering::InitImgui(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImVector<ImWchar> ranges;
  ImFontGlyphRangesBuilder builder;
  builder.AddText(u8"ěščřžýáíéůúĚŠČŘŽÝÁÍÉÚ"); // Add a string (here "Hello world" contains 7
                                  // unique characters)
  builder.AddRanges(
      io.Fonts->GetGlyphRangesDefault()); // Add one of the default ranges
  builder.BuildRanges(&ranges); // Build the final result (ordered ranges with
                                // all the unique characters submitted)

  io.Fonts->AddFontFromFileTTF("../../res/OpenSans.ttf", 18.0f, nullptr,
                               ranges.Data);
  io.Fonts->Build(); // Build the atlas while 'ranges' is still in scope and not
                     // deleted.

  io.Fonts->Build();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
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
