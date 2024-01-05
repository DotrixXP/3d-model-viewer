
#pragma once
#include "../Core/Window.hpp"

struct KeyboardEvent {
  int key;
  int keyAction;
};

class Keyboard {
private:
  static KeyboardEvent m_keyboardEvent;

public:
  Keyboard() = delete;
  ~Keyboard() = delete;
  static bool IsKeyPressed(int key);
  static bool IsKeyReleased(int key);
  static KeyboardEvent &GetKeyCallback();
  static void GetGLFWKeyCallback(int key, int action);
};
