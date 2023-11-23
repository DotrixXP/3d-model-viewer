#pragma once
#include "Window.hpp"
#include "../Tools/Log.hpp"
#include <iostream>
#include "Window.hpp"
#include "../Graphics/ImguiRendering.hpp"
#include "../Graphics/Shader.hpp"
#include "OpenglAPI.hpp"
#include "OpenglData.hpp"

class Core {
private:
  static glm::vec3 s_backgroundColor;
  static GLuint s_render_target_texture; // Tektura, do které se vyrenderuje scéna
  static GLuint s_render_target_framebuffer; // Framebuffer, do kterého se vyrenderuje scéna
  static void StartRenderingToTexture();
  static void FinishRenderingToTexture();
  static void CreateRenderTargets(int32_t width, int32_t height);
public:

  static bool InitEngine();
  static void OnRenderStart();
  static void OnRenderEnd();
  static void UpdateVariables();
  static void ClearBuffers();
  static void SetBackgroundColor(const glm::vec3 &color);
  static GLuint GetRenderTargetTexture();
  static GLuint GetRenderTargetFramebuffer();
};
