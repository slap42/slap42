#pragma once

#include <enet/enet.h>

namespace Slap42 {
namespace Client {

void OnServerDisconnect(ENetEvent& evt);
void OnMessageRecv(ENetEvent& evt);

}
}
