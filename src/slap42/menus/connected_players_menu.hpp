#pragma once

#include "networking/message_types.hpp"

namespace Slap42 {
namespace ConnectedPlayersMenu {

void SetIdAndCapacity(peer_id id, int capacity);

void Render();

}
}
