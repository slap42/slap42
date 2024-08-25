#pragma once

namespace Slap42 {

enum class MenuState {
  kNone,
  kHostJoinMenu,
  kHostMenu,
  kJoinMenu,
  kJoinAsyncMenu,
  kPauseMenu,
  kConnectedPlayersMenu,
  kErrorMenu,
};

}
