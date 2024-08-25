#pragma once

#include <cstdint>

namespace Slap42 {

enum class ServerState {
  kStopped,
  kRunning,
  kError,
};

namespace Server {

void StartServer(uint16_t port, int capacity, uint32_t seed);
void StopServer();

ServerState GetState();

}
}
