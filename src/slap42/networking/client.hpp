#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>
#include "networking/message_types.hpp"
#include "peer_data.hpp"

namespace Slap42 {
namespace Client {

bool ClientConnect(const char* hostname, uint16_t port);
void ClientDisconnect();
void ClientPollMessages();

void ClientSendPositionUpdate(const glm::vec3& pos, const glm::vec2& rot);
void ClientSendChatMessage(const std::string& msg);

std::unordered_map<peer_id, std::shared_ptr<PeerData>>* GetPeerData();


}
}
