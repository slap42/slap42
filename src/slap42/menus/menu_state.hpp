#pragma once

namespace Slap42 {

enum class MenuState {
  kNone,
  kHostJoinMenu,
  kHostErrorMenu,
  kJoinMenu,
  kJoinErrorMenu,
  kJoinAsyncMenu,
  kPauseMenu,
};

}
