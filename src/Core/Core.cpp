#include "Core.hpp"
#include "CameraSystem.hpp"

glm::vec3 Core::s_backgroundColor(1.0f, 1.0f, 1.0f);
GLuint Core::s_render_target_texture = 0;
GLuint Core::s_render_target_framebuffer = 0;
GLuint Core::s_render_target_depthbuffer = 0;

GLuint Core::s_render_target_multisample_texture = 0;
GLuint Core::s_render_target_multisample_framebuffer = 0;
uint8_t Core::s_msaaSamples = 8;

glm::vec2 Core::s_framebufferSize = glm::vec2(500, 500);

constexpr uint32_t DEFAULT_WIDTH = 1280;
constexpr uint32_t DEFAULT_HEIGHT = 720;

void Core::SetMsaaSamples(uint8_t samples) {
    if (samples < 1 || samples > 8 || samples % 2 != 0) {
        Log::Error("Invalid MSAA samples count");
        return;
    }

    s_msaaSamples = samples;
    RecreateMultisampleTargetTexture(s_framebufferSize.x, s_framebufferSize.y);
}

uint8_t Core::GetMsaaSamples() {
    return s_msaaSamples;
}

bool Core::InitEngine() {
    auto result = OpenglAPI::InitOpenglAPI();
    CreateMultisampleRenderTargets(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    CreateRenderTargets(DEFAULT_WIDTH, DEFAULT_HEIGHT);
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

void Core::CreateMultisampleRenderTargets(int32_t width, int32_t height) {
    glGenTextures(1, &s_render_target_multisample_texture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, s_render_target_multisample_texture);
    glTexImage2DMultisample(
        GL_TEXTURE_2D_MULTISAMPLE, s_msaaSamples, GL_RGB, width, height, GL_TRUE);

    glGenFramebuffers(1, &s_render_target_multisample_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, s_render_target_multisample_framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D_MULTISAMPLE,
                           s_render_target_multisample_texture,
                           0);

    glGenRenderbuffers(1, &s_render_target_depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, s_render_target_depthbuffer);
    glRenderbufferStorageMultisample(
        GL_RENDERBUFFER, s_msaaSamples, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, s_render_target_depthbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Log::Error("Error while creating multisample framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Core::RecreateMultisampleTargetTexture(int32_t width, int32_t height) {
    glDeleteTextures(1, &s_render_target_multisample_texture);
    glDeleteFramebuffers(1, &s_render_target_multisample_framebuffer);
    glDeleteRenderbuffers(1, &s_render_target_depthbuffer);
    CreateMultisampleRenderTargets(width, height);
}

void Core::CreateRenderTargets(int32_t width, int32_t height) {
    glGenTextures(1, &s_render_target_texture);
    glBindTexture(GL_TEXTURE_2D, s_render_target_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &s_render_target_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, s_render_target_framebuffer);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, s_render_target_texture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log::Error("There was a error while creating framebuffer");
}

void Core::RecreateTargetTexture(int32_t width, int32_t height) {
    std::cout << width << " " << height << "\n";
    glDeleteTextures(1, &s_render_target_texture);
    glDeleteFramebuffers(1, &s_render_target_framebuffer);

    CreateRenderTargets(width, height);
    RecreateMultisampleTargetTexture(width, height);
}

void Core::StartRenderingToTexture(glm::vec2 viewportSize) {
    if (s_framebufferSize != viewportSize) {
        s_framebufferSize = viewportSize;
        Core::RecreateTargetTexture(s_framebufferSize.x, s_framebufferSize.y);
    }
    glViewport(0, 0, s_framebufferSize.x, s_framebufferSize.y);
    glBindFramebuffer(GL_FRAMEBUFFER, s_render_target_multisample_framebuffer);
    Core::ClearBuffers();
}

GLuint Core::GetRenderTargetTexture() {
    return s_render_target_texture;
}

GLuint Core::GetRenderTargetFramebuffer() {
    return s_render_target_framebuffer;
};

void Core::FinishRenderingToTexture() {
    auto windowSize = glm::vec2(Window::GetWindowWidth(), Window::GetWindowHeight());
    glViewport(0, 0, windowSize.x, windowSize.y);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, s_render_target_multisample_framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s_render_target_framebuffer);
    glBlitFramebuffer(0,
                      0,
                      s_framebufferSize.x,
                      s_framebufferSize.y,
                      0,
                      0,
                      s_framebufferSize.x,
                      s_framebufferSize.y,
                      GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Core::ClearBuffers(true);
}

void Core::UpdateVariables() {
    ImguiRendering::UpdateImgui();
}

void Core::ClearBuffers(bool blackBackground) {
    if (blackBackground)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    else
        glClearColor(s_backgroundColor.x, s_backgroundColor.y, s_backgroundColor.z, 1.0f);

    if (OpenglData::GetStencilTesting() && OpenglData::GetDepthTesting()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_TEST);

        if (OpenglData::GetColorBuffer())
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_TEST);
        else
            glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_TEST);
    }
    else if (OpenglData::GetStencilTesting() && !OpenglData::GetDepthTesting()) {
        if (OpenglData::GetColorBuffer())
            glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_TEST);
        else
            glClear(GL_STENCIL_TEST);
    }
    else if (!OpenglData::GetStencilTesting() && OpenglData::GetDepthTesting()) {
        if (OpenglData::GetColorBuffer())
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        else
            glClear(GL_DEPTH_BUFFER_BIT);
    }
    else if (!OpenglData::GetStencilTesting() && !OpenglData::GetDepthTesting()) {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Core::SetBackgroundColor(const glm::vec3& color) {
    s_backgroundColor = color;
}
