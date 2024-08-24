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

bool Connect(const char* hostname, uint16_t port);
void InterruptConnectAttempt();
void Disconnect();
void PollMessages();

void SendPositionUpdate(const glm::vec3& pos, const glm::vec2& rot);
void SendChatMessage(const std::string& msg);
void SendKickPlayer(peer_id id);

std::unordered_map<peer_id, std::shared_ptr<PeerData>>* GetPeerData();


}
}
