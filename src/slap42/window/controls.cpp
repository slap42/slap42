#include "controls.hpp"

#include <unordered_map>
#include <GLFW/glfw3.h>
#include "window/window.hpp"

namespace Slap42 {
namespace Controls {

static float mouse_delta_x = 0.0f, mouse_delta_y = 0.0f;

enum class ButtonBindDevice {
  kKeyboard,
  kMouse,
  kGamepad,
};

struct ButtonData {
  int binding;
  ButtonBindDevice device = ButtonBindDevice::kKeyboard;
  int down_frames = 0;
};

static std::unordered_map<Button, ButtonData> button_data = {
  { Button::kMoveForward,     { GLFW_KEY_W,              ButtonBindDevice::kKeyboard } },
  { Button::kMoveBack,        { GLFW_KEY_S,              ButtonBindDevice::kKeyboard } },
  { Button::kMoveLeft,        { GLFW_KEY_A,              ButtonBindDevice::kKeyboard } },
  { Button::kMoveRight,       { GLFW_KEY_D,              ButtonBindDevice::kKeyboard } },
  { Button::kJump,            { GLFW_KEY_SPACE,          ButtonBindDevice::kKeyboard } },
  { Button::kCrouch,          { GLFW_KEY_LEFT_SHIFT,     ButtonBindDevice::kKeyboard } },
  { Button::kRotateUp,        { GLFW_KEY_UP,             ButtonBindDevice::kKeyboard } },
  { Button::kRotateDown,      { GLFW_KEY_DOWN,           ButtonBindDevice::kKeyboard } },
  { Button::kRotateLeft,      { GLFW_KEY_LEFT,           ButtonBindDevice::kKeyboard } },
  { Button::kRotateRight,     { GLFW_KEY_RIGHT,          ButtonBindDevice::kKeyboard } },
  { Button::kOpenChatMenu,    { GLFW_KEY_T,              ButtonBindDevice::kKeyboard } },
  { Button::kOpenMainMenu,    { GLFW_KEY_ESCAPE,         ButtonBindDevice::kKeyboard } },
  { Button::kActionPrimary,   { GLFW_MOUSE_BUTTON_LEFT,  ButtonBindDevice::kMouse    } },
  { Button::kActionSecondary, { GLFW_MOUSE_BUTTON_RIGHT, ButtonBindDevice::kMouse    } },
};

static InputState state = InputState::kMouseFree;

InputState GetInputState() {
  return state;
}

void SetInputState(InputState s) {
  state = s;

  GLFWwindow* window = Window::GetGlfwWindow();

  if (s == InputState::kMouseGrabbed) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
      glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
  }
  else {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    if (glfwRawMouseMotionSupported()) {
      glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
    }
  }
}

bool IsInputStateInMenu() {
  return state != InputState::kMouseFree && state != InputState::kMouseGrabbed;
}

void Update() {
  GLFWwindow* window = Window::GetGlfwWindow();

  // Mouse delta
  double mouse_x = 0.0, mouse_y = 0.0;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);
  static double mouse_old_x = mouse_x, mouse_old_y = mouse_y;
  mouse_delta_x = (float)(mouse_x - mouse_old_x);
  mouse_delta_y = (float)(mouse_y - mouse_old_y);
  mouse_old_x = mouse_x;
  mouse_old_y = mouse_y;

  // Button states
  for (auto& b : button_data) {
    switch (b.second.device) {
    case ButtonBindDevice::kKeyboard:
      if (glfwGetKey(window, b.second.binding)) { ++b.second.down_frames; }
      else { b.second.down_frames = 0; }
      break;

    case ButtonBindDevice::kMouse:
      if (glfwGetMouseButton(window, b.second.binding)) { ++b.second.down_frames; }
      else { b.second.down_frames = 0; }
      break;

    case ButtonBindDevice::kGamepad:
      // TODO
      break;
    }
  }
}

bool IsButtonDown(Button button) {
  return button_data[button].down_frames;
}

bool IsButtonPressed(Button button) {
  return button_data[button].down_frames == 1;
}


float GetMouseDeltaX() {
  return mouse_delta_x;
}

float GetMouseDeltaY() {
  return mouse_delta_y;
}

}
}

