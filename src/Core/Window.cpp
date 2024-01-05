#include "Window.hpp"
#include "CameraSystem.hpp"

GLFWwindow *Window::s_window = nullptr;
Display *Window::s_display = nullptr;
bool Window::s_isCursorVisible = true;
bool Window::s_isFullscreenEnabled = false;
bool Window::s_Vsync = false;
uint32_t Window::s_windowHeight = 720;
uint32_t Window::s_windowWidth = 1280;

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  CameraSystem::GetInstance().ProcessMouseInput(xpos, ypos);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  CameraSystem::GetInstance().ProcessMouseScrollInput(xoffset, yoffset);
}

void Window::OnRenderStart() {
  glfwSwapInterval(Window::GetVsyncBool()); // Vsync
  framebuffer_size_callback(Window::GetGLFWwindowRef(),
                            Window::GetWindowWidth(),
                            Window::GetWindowHeight());
  if(s_isCursorVisible)
    glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  
  if(!s_isCursorVisible)
    glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::OnRenderEnd() {
  glfwSwapBuffers(Window::GetGLFWwindowRef());
  glfwPollEvents();
}

bool Window::InitWindow(const uint32_t windowWidth, const uint32_t windowHeight,
                        const std::string &windowTitle) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  Window::s_windowWidth = windowWidth;
  Window::s_windowHeight = windowHeight;

  if (!glfwInit())
    return false;

  s_window = glfwCreateWindow(Window::s_windowWidth, Window::s_windowHeight,
                              windowTitle.c_str(), NULL, NULL);

  if (!s_window) {
    glfwTerminate();
    return false;
  }

  s_display = new Display;
  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  s_display->screenWidth = mode->width;
  s_display->screenHeight = mode->height;

  glfwMakeContextCurrent(s_window);

  glfwSetCursorPosCallback(s_window, mouse_callback);
  glfwSetScrollCallback(s_window, scroll_callback);

  glfwSetFramebufferSizeCallback(s_window, framebuffer_size_callback);

  Core::InitEngine();

  ImguiRendering::InitImgui(s_window);

  return true;
}

void Window::ChangeWindowTitle(const std::string &windowTitle) {
  glfwSetWindowTitle(s_window, windowTitle.c_str());
}

void Window::DestroyWindow() {
  glfwDestroyWindow(s_window);
  glfwTerminate();
  delete s_display;
  s_display = nullptr;

  ImguiRendering::ShutdownImguiRendering();
}

int Window::WindowShouldClose() { return glfwWindowShouldClose(s_window); }

void Window::SetFullscreen() {
  glfwSetWindowAttrib(s_window, GLFW_DECORATED, GLFW_FALSE);
  glfwSetWindowPos(s_window, 0, 0);
  glfwSetWindowSize(s_window, s_display->screenWidth, s_display->screenHeight);

  Window::s_isFullscreenEnabled = true;
}

void Window::SetWindowed() {
  glfwSetWindowAttrib(s_window, GLFW_DECORATED, GLFW_TRUE);
  glfwSetWindowPos(s_window, s_display->screenWidth / 6,
                   s_display->screenHeight / 6);
  glfwSetWindowSize(s_window, 1280, 720);

  Window::s_isFullscreenEnabled = false;
}
void Window::SetVsync(bool value) { s_Vsync = value; }
void Window::SetWindowSize(uint32_t windowWidth, uint32_t windowHeight) {
  s_windowWidth = windowWidth;
  s_windowHeight = windowHeight;
}

void Window::SetCursorVisible(bool visible) { s_isCursorVisible = visible; }

const bool Window::GetVsyncBool() { return s_Vsync; }
const int Window::GetWindowWidth() { return s_windowWidth; }
const int Window::GetWindowHeight() { return s_windowHeight; }
const bool Window::GetCursorVisibility() { return s_isCursorVisible; }
GLFWwindow *Window::GetGLFWwindowRef() { return s_window; }

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  int _height, _width;

  if (width != Window::GetWindowWidth() ||
      height != Window::GetWindowHeight()) {
    glfwGetWindowSize(Window::GetGLFWwindowRef(), &_width, &_height);
    Window::SetWindowSize(_width, _height);

    Log::LogWarning("Aktualizuje se Framebuffer & Viewport velikost");

    glViewport(0, 0, Window::GetWindowWidth(), Window::GetWindowHeight());
  }

  glfwGetWindowSize(Window::GetGLFWwindowRef(), &_width, &_height);
  Window::SetWindowSize(_width, _height);
}
