#pragma once

#include <string>
#include "networking/message_types.hpp"

namespace Slap42 {
namespace ChatPanel {

void AddChatMessage(peer_id sender, const std::string& msg);

void Render();

}
}
