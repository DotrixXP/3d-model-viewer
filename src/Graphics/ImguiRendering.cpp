#include "ImguiRendering.hpp"

void ImguiRendering::InitImgui(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  const std::string fontPath = std::string(GLOBAL_PATH) + std::string("res/font.ttf");
  std::cout << "Font path: " << fontPath << std::endl;
  // Vytvoříme si instanci ImGuiIO
  ImGuiIO &io = ImGui::GetIO();
  ImVector<ImWchar> ranges;
  ImFontGlyphRangesBuilder builder;
  builder.AddText(u8"áčďéěíňóřšťúůýžÁČĎÉĚÍŇÓŘŠŤÚŮÝŽ"); // Přidáme řetězec
  builder.AddRanges(io.Fonts->GetGlyphRangesDefault()); // Přidáme jednu z výchozích sad
  builder.BuildRanges(&ranges); // Sestavíme výsledný výsledek (seřazené sady se
                                // všemi jedinečnými znaky)

  auto font =
      io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 20, nullptr, ranges.Data);

  if (font == nullptr) {
    // Font se nepodařilo načíst, zobrazíme chybovou zprávu
    Log::LogError("Nepodařilo se načíst font: " + std::string(fontPath));
  } else {
    // Font se načetl úspěšně, nastavíme ho jako výchozí
    io.FontDefault = font;
  }

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
