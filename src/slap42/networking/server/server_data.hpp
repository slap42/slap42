#pragma once

#include <memory>
#include <unordered_map>
#include <enet/enet.h>
#include "networking/message_types.hpp"
#include "networking/peer_data.hpp"

namespace Slap42 {
namespace Server {

extern ENetHost* server;
extern std::unordered_map<peer_id, std::shared_ptr<PeerData>> peer_data;

}
}
