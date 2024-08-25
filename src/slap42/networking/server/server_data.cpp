#include "server_data.hpp"

namespace Slap42 {
namespace Server {

ENetHost* server;
std::unordered_map<peer_id, std::shared_ptr<PeerData>> peer_data;

}
}
