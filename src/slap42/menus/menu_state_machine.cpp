#include "menu_state_machine.hpp"

#include "host_join_menu.hpp"
#include "join_menu.hpp"
#include "join_error_menu.hpp"
#include "join_async_menu.hpp"
#include "pause_menu.hpp"
#include "window/controls.hpp"

namespace Slap42 {
namespace MenuStateMachine {

MenuState state = MenuState::kHostJoinMenu;

void SetState(MenuState s) {
  if (state == MenuState::kNone) {
    Controls::CacheCursorGrabbed();
  }

  state = s;

  if (state == MenuState::kNone) {
    Controls::SetInputStateWithCachedCursor();
  }

  if (state == MenuState::kPauseMenu) {
    Controls::SetInputState(InputState::kNonBlockingMenu);
  }
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

  case MenuState::kPauseMenu:
    PauseMenu::Render();
    break;

  }
}

}
}
