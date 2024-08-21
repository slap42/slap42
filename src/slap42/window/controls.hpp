#pragma once

namespace Slap42 {
namespace Controls {

enum class Button {
  kMoveForward,
  kMoveBack,
  kMoveLfet,
  kMoveRight,
};

void Update();

bool IsButtonDown(Button button);
bool IsButtonPressed(Button button);

float GetMouseDeltaX();
float GetMouseDeltaY();

}
}
