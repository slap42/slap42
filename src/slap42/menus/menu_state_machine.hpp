#pragma once

#include "menu_state.hpp"

namespace Slap42 {
namespace MenuStateMachine {

void SetState(MenuState state);
MenuState GetState();

void Update();

void Render();

}
}
