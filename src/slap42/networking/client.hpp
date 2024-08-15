#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>
#include "peer_data.hpp"

namespace Slap42 {
namespace Client {

bool ClientConnect(const char* hostname, uint16_t port);
void ClientDisconnect();
void ClientPollMessages();

void ClientSendPositionUpdate(const glm::vec3& pos, const glm::vec2& rot);

std::unordered_map<uint64_t, std::shared_ptr<PeerData>>* GetPeerData();


}
}
