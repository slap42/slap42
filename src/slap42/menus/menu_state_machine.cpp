#include "menu_state_machine.hpp"

#include "error_menu.hpp"
#include "host_join_menu.hpp"
#include "host_menu.hpp"
#include "join_menu.hpp"
#include "join_async_menu.hpp"
#include "pause_menu.hpp"
#include "connected_players_menu.hpp"
#include "window/controls.hpp"

namespace Slap42 {
namespace MenuStateMachine {

static MenuState state = MenuState::kHostJoinMenu;
static bool state_changed_this_frame = false;

void SetState(MenuState s) {
  if (state_changed_this_frame) return;
  state_changed_this_frame = true;
  
  if (state == MenuState::kNone) {
    Controls::CacheCursorGrabbed();
  }

  state = s;

  if (state == MenuState::kNone) {
    Controls::SetInputStateWithCachedCursor();
  }
  else if (state == MenuState::kHostMenu) {
    HostMenu::GenerateRandomSeed();
  }
  else if (state == MenuState::kPauseMenu) {
    Controls::SetInputState(InputState::kNonBlockingMenu);
  }
  else if (state == MenuState::kErrorMenu) {
    Controls::SetInputState(InputState::kBlockingMenuNotInGame);
  }
}

MenuState GetState() {
  return state;
}

void Update() {
  state_changed_this_frame = false;
}


void Render() {
  switch (state) {
  case MenuState::kNone:
    return;

  case MenuState::kHostJoinMenu:
    HostJoinMenu::Render();
    break;

  case MenuState::kHostMenu:
    ErrorMenu::SetReturnMenu(MenuState::kHostMenu);
    HostMenu::Render();
    break;

  case MenuState::kJoinMenu:
    ErrorMenu::SetReturnMenu(MenuState::kJoinMenu);
    JoinMenu::Render();
    break;

  case MenuState::kJoinAsyncMenu:
    JoinAsyncMenu::Render();
    break;

  case MenuState::kPauseMenu:
    PauseMenu::Render();
    break;
      
  case MenuState::kConnectedPlayersMenu:
    ConnectedPlayersMenu::Render();
    break;

  case MenuState::kErrorMenu:
    ErrorMenu::Render();
    break;

  }
}

}
}
