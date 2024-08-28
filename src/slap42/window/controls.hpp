#pragma once

namespace Slap42 {

enum class Button {
  kMoveForward,
  kMoveBack,
  kMoveLeft,
  kMoveRight,

  kRotateUp,
  kRotateDown,
  kRotateLeft,
  kRotateRight,

  kJump,
  kCrouch,
  
  kToggleFlying,

  kOpenChatMenu,
  kOpenMainMenu,

  kActionPrimary,   // Default left mouse button
  kActionSecondary, // Default right mouse button
};

enum class InputState {
  kMouseFree,
  kMouseGrabbed,
  kNonBlockingMenu,
  kBlockingMenuInGame,
  kBlockingMenuNotInGame,
};

namespace Controls {

void Update();

bool IsButtonDown(Button button);
bool IsButtonPressed(Button button);

float GetMouseDeltaX();
float GetMouseDeltaY();

InputState GetInputState();
void SetInputState(InputState s);

bool IsInPlayerInputState();
void CacheCursorGrabbed();
void SetInputStateWithCachedCursor();

}
}
