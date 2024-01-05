#pragma once
#include "../Graphics/Shader.hpp"
#include "Core.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;

struct Display {
  uint32_t screenHeight;
  uint32_t screenWidth;
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

class Window {
private:
  static bool s_Vsync;
  static Display *s_display;
  static uint32_t s_windowHeight;
  static uint32_t s_windowWidth;
  static bool s_isCursorVisible;
  static bool s_isFullscreenEnabled;
  static GLFWwindow *s_window;

protected:
  static void OnRenderStart();
  static void OnRenderEnd();

public:
  Window(const Window &) = delete;
  Window() = delete;
  ~Window() = delete;

  static bool InitWindow(const uint32_t screenWidth,
                         const uint32_t screenHeight,
                         const std::string &windowTitle);
  static void DestroyWindow();
  static void ChangeWindowTitle(const std::string &windowTitle);
  static int WindowShouldClose();
  // Settery
  static void SetFullscreen();
  static void SetWindowed();
  static void SetVsync(bool value);
  static void SetWindowSize(uint32_t windowWidth, uint32_t windowHeight);
  static void SetCursorVisible(bool visible);
  // Gettery
  static const bool GetVsyncBool();
  static const int GetWindowWidth();
  static const int GetWindowHeight();
  static const bool GetCursorVisibility();
  static GLFWwindow *GetGLFWwindowRef();

  friend class Core;
};
