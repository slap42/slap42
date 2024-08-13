#pragma once

#include <cstdint>

namespace Slap42 {

void ClientConnect(const char* hostname, uint16_t port);
void ClientDisconnect();
void ClientUpdate();

}
