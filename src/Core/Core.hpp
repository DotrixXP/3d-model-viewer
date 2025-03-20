#pragma once
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
    static GLuint s_render_target_texture;
    static GLuint s_render_target_framebuffer;

    static GLuint s_render_target_multisample_texture;
    static GLuint s_render_target_multisample_framebuffer;
    static GLuint s_render_target_depthbuffer;

    static glm::vec2 s_framebufferSize;
    static uint8_t s_msaaSamples;
    static void CreateRenderTargets(int32_t width, int32_t height);
    static void RecreateTargetTexture(int32_t width, int32_t height);

  public:
    static void CreateMultisampleRenderTargets(int32_t width, int32_t height);
    static void RecreateMultisampleTargetTexture(int32_t width, int32_t height);
    static void StartRenderingToTexture(glm::vec2 viewportSize);
    static void FinishRenderingToTexture();
    static bool InitEngine();
    static void SetMsaaSamples(uint8_t samples);
    static void OnRenderStart();
    static void OnRenderEnd();
    static void UpdateVariables();
    static void ClearBuffers(bool whiteBackground = false);
    static void SetBackgroundColor(const glm::vec3& color);
    static GLuint GetRenderTargetTexture();
    static uint8_t GetMsaaSamples();
    static GLuint GetRenderTargetFramebuffer();
};
