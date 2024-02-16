#include "Core.hpp"
#include "CameraSystem.hpp"

glm::vec3 Core::s_backgroundColor(1.0f, 1.0f, 1.0f);
GLuint Core::s_render_target_texture;
GLuint Core::s_render_target_framebuffer;
GLuint Core::s_render_target_depthbuffer;

glm::vec2 Core::s_framebufferSize = glm::vec2(500, 500);
bool Core::InitEngine() {
  auto result = OpenglAPI::InitOpenglAPI();
  CreateRenderTargets(531, 561); // TODO:: Upravit na const defaultní velikost
  return result;
}

void Core::OnRenderStart() {
  Window::OnRenderStart();
  ImguiRendering::UpdateImgui();
}

void Core::OnRenderEnd() {
  OpenglAPI::OnRenderEnd();
  ImguiRendering::OnRenderEnd();
  Window::OnRenderEnd(); // Výměna bufferù
}

void Core::CreateRenderTargets(int32_t width, int32_t height) {
  // Vytvoření textury
  glGenTextures(1, &s_render_target_texture);
  glBindTexture(GL_TEXTURE_2D, s_render_target_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Vytvoření depth bufferu
  glGenRenderbuffers(1, &s_render_target_depthbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, s_render_target_depthbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

  // Vytvoření framebufferu
  glGenFramebuffers(1, &s_render_target_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, s_render_target_framebuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         s_render_target_texture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, s_render_target_depthbuffer);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    Log::LogError("There was a error while creating framebuffer");
}

void Core::RecreateTargetTexture(int32_t width, int32_t height) {
  std::cout << width << " " << height << "\n";
  glGenTextures(1, &s_render_target_texture);
  glBindTexture(GL_TEXTURE_2D, s_render_target_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindRenderbuffer(GL_RENDERBUFFER, s_render_target_depthbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

  glBindFramebuffer(GL_FRAMEBUFFER, s_render_target_framebuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         s_render_target_texture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, s_render_target_depthbuffer);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    Log::LogError("There was a error while creating framebuffer");
}

void Core::StartRenderingToTexture(glm::vec2 viewportSize) {
  if (s_framebufferSize != viewportSize) {
    s_framebufferSize = viewportSize;
    Core::RecreateTargetTexture(s_framebufferSize.x, s_framebufferSize.y);
  }
  glViewport(0, 0, s_framebufferSize.x, s_framebufferSize.y);
  glBindFramebuffer(GL_FRAMEBUFFER, s_render_target_framebuffer);
  Core::ClearBuffers();
}

GLuint Core::GetRenderTargetTexture() { return s_render_target_texture; }

GLuint Core::GetRenderTargetFramebuffer() {
  return s_render_target_framebuffer;
};

void Core::FinishRenderingToTexture() {
  auto windowSize =
      glm::vec2(Window::GetWindowWidth(), Window::GetWindowHeight());
  glViewport(0, 0, windowSize.x, windowSize.y);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  Core::ClearBuffers(true);
}

void Core::UpdateVariables() { ImguiRendering::UpdateImgui(); }

void Core::ClearBuffers(bool blackBackground) {
  if (blackBackground)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  else
    glClearColor(s_backgroundColor.x, s_backgroundColor.y, s_backgroundColor.z,
                 1.0f);

  if (OpenglData::GetStencilTesting() && OpenglData::GetDepthTesting()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_TEST);

    if (OpenglData::GetColorBuffer())
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_TEST);
    else
      glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_TEST);

  } else if (OpenglData::GetStencilTesting() &&
             !OpenglData::GetDepthTesting()) {
    if (OpenglData::GetColorBuffer())
      glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_TEST);
    else
      glClear(GL_STENCIL_TEST);

  } else if (!OpenglData::GetStencilTesting() &&
             OpenglData::GetDepthTesting()) {
    if (OpenglData::GetColorBuffer())
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    else
      glClear(GL_DEPTH_BUFFER_BIT);

  } else if (!OpenglData::GetStencilTesting() &&
             !OpenglData::GetDepthTesting()) {
    glClear(GL_COLOR_BUFFER_BIT);
  }
}

void Core::SetBackgroundColor(const glm::vec3 &color) {
  s_backgroundColor = color;
}
