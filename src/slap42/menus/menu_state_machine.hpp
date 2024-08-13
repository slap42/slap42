#pragma once

#include "menu_state.hpp"

namespace Slap42 {
namespace MenuStateMachine {

void Render();

void SetState(MenuState state);
MenuState GetState();

}
}
