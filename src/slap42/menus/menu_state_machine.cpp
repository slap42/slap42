#include "menu_state_machine.hpp"

#include "host_join_menu.hpp"
#include "join_menu.hpp"

namespace Slap42 {

void MenuStateMachine::Render() {
  switch (state) {
  case MenuState::kNone:
    return;

  case MenuState::kHostJoinMenu:
    HostJoinMenu::Render(&state);
    break;

  case MenuState::kJoinMenu:
    JoinMenu::Render(&state);
    break;
  }
}

}
