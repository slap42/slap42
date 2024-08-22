#include "menu_state_machine.hpp"

#include "host_join_menu.hpp"
#include "join_menu.hpp"
#include "join_error_menu.hpp"
#include "join_async_menu.hpp"

namespace Slap42 {
namespace MenuStateMachine {

MenuState state = MenuState::kHostJoinMenu;

void SetState(MenuState s) {
  state = s;
}

MenuState GetState() {
  return state;
}

void Render() {
  switch (state) {
  case MenuState::kNone:
    return;

  case MenuState::kHostJoinMenu:
    HostJoinMenu::Render();
    break;

  case MenuState::kJoinMenu:
    JoinMenu::Render();
    break;

  case MenuState::kJoinErrorMenu:
    JoinErrorMenu::Render();
    break;

  case MenuState::kJoinAsyncMenu:
    JoinAsyncMenu::Render();
    break;

  }
}

}
}
