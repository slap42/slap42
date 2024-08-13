#pragma once

#include "menu_state.hpp"

namespace Slap42 {

class MenuStateMachine {
public:
  void Render();

public:
  inline MenuState GetState() const { return state; };

private:
  MenuState state = MenuState::kHostJoinMenu;
};

}
