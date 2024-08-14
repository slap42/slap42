#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace Slap42 {
namespace Client {

bool ClientConnect(const char* hostname, uint16_t port);
void ClientDisconnect();
void ClientPollMessages();

void ClientSendPositionUpdate(const glm::vec3& pos, const glm::vec2& rot);

}
}
