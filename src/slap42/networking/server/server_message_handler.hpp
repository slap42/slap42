#pragma once

#include <enet/enet.h>

namespace Slap42 {
namespace Server {

void OnClientConnect(ENetEvent& evt);
void OnClientDisconnect(ENetEvent& evt);
void OnMessageRecv(ENetEvent& evt);

}
}
