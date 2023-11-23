#include "WindowSystem.hpp"

void WindowSystem::RenderWindows() {
  ImGui::Begin("Viewport");

  auto winSize = ImGui::GetWindowSize();
  winSize.y -= 40;

  ImGui::Image(
      (void *)(intptr_t)Core::GetRenderTargetTexture(),
    winSize);
  ImGui::End();
}
