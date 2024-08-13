#pragma once

#include <cstdint>

namespace Slap42 {
namespace Client {

bool ClientConnect(const char* hostname, uint16_t port);
void ClientDisconnect();
void ClientUpdate();

}
}
