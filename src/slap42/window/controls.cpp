#include "controls.hpp"

#include <GLFW/glfw3.h>
#include "window/window.hpp"

namespace Slap42 {
namespace Controls {

static float mouse_delta_x = 0.0f, mouse_delta_y = 0.0f;

void Update() {
  double mouse_x = 0.0, mouse_y = 0.0;
  glfwGetCursorPos(Window::GetGlfwWindow(), &mouse_x, &mouse_y);
  static double mouse_old_x = mouse_x, mouse_old_y = mouse_y;
  mouse_delta_x = (float)(mouse_x - mouse_old_x);
  mouse_delta_y = (float)(mouse_y - mouse_old_y);
  mouse_old_x = mouse_x;
  mouse_old_y = mouse_y;
}

bool IsButtonDown(Button button) {
  return false;
}

bool IsButtonPressed(Button button) {
  return false;
}


float GetMouseDeltaX() {
  return mouse_delta_x;
}

float GetMouseDeltaY() {
  return mouse_delta_y;
}

}
}

