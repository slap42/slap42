#pragma once

namespace Slap42 {

enum class ServerState {
  kStopped,
  kRunning,
  kError,
};

namespace Server {

void StartServer();
void StopServer();

ServerState GetState();

}
}
