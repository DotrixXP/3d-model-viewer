#include "Keyboard.hpp"

KeyboardEvent Keyboard::m_keyboardEvent;

bool Keyboard::IsKeyPressed(int key) {
  if (glfwGetKey(Window::GetGLFWwindowRef(), key) == GLFW_PRESS)
    return true;
  else
    return false;
}

bool Keyboard::IsKeyReleased(int key) {
  if (glfwGetKey(Window::GetGLFWwindowRef(), key) == GLFW_RELEASE)
    return true;
  else
    return false;
}

void Keyboard::GetGLFWKeyCallback(int key, int action) {
  m_keyboardEvent.key = key;
  m_keyboardEvent.keyAction = action;
}

KeyboardEvent &Keyboard::GetKeyCallback() { return m_keyboardEvent; }
