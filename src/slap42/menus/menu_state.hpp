#pragma once

namespace Slap42 {

enum class MenuState {
  kNone,
  kHostJoinMenu,
  kJoinMenu,
  kJoinErrorMenu,
  kJoinAsyncMenu,
  kPauseMenu,
};

}
