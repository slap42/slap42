#pragma once

namespace Slap42 {

enum class MenuState {
  kNone,
  kHostJoinMenu,
  kHostMenu,
  kHostErrorMenu,
  kJoinMenu,
  kJoinErrorMenu,
  kJoinAsyncMenu,
  kPauseMenu,
  kConnectedPlayersMenu,
};

}
