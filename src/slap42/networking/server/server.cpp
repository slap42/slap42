#include "server.hpp"

#include <cstdio>
#include <thread>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <enet/enet.h>
#include <glm/glm.hpp>
#include "menus/error_menu.hpp"
#include "networking/disconnect_reasons.hpp"
#include "networking/message_types.hpp"
#include "networking/message_serializer.hpp"
#include "networking/peer_data.hpp"
#include "server_data.hpp"
#include "server_message_handler.hpp"
#include "utils/hash.hpp"

namespace Slap42 {
namespace Server {

static std::mutex server_state_mutex;
static ServerState state = ServerState::kStopped;

static std::thread* server_thread;

static void SetState(ServerState s) {
  std::scoped_lock sl(server_state_mutex);
  state = s;
}

ServerState GetState() {
  std::scoped_lock sl(server_state_mutex);
  return state;
}

static void RunServer(uint16_t port) {
  ENetAddress address { };
  address.host = ENET_HOST_ANY;
  address.port = port;

  server = enet_host_create(&address, 32, 1, 0, 0);
  if (!server) {
    ErrorMenu::SetErrorMessage("[SERVER] Failed to start server: enet_host_create failed.\nThe chosen port might already be in use by another application.");
    SetState(ServerState::kError);
    return;
  }
  else {
    SetState(ServerState::kRunning);
  }

  ENetEvent evt;
  while (GetState() == ServerState::kRunning) {
    while (enet_host_service(server, &evt, 0) > 0) {
      
      switch (evt.type) {
        case ENET_EVENT_TYPE_CONNECT: {
          OnClientConnect(evt);
          break;
        }

        case ENET_EVENT_TYPE_DISCONNECT: {
          OnClientDisconnect(evt);
          break;
        }

        case ENET_EVENT_TYPE_RECEIVE: {
          OnMessageRecv(evt);
          break;
        }
          
        case ENET_EVENT_TYPE_NONE:
          break;
      }
    }
  }

  enet_host_destroy(server);
}

void StartServer(uint16_t port) {
  if (GetState() != ServerState::kStopped) return;
  server_thread = new std::thread(RunServer, port);
}

void StopServer() {
  if (GetState() == ServerState::kStopped) return;
  SetState(ServerState::kStopped);
  server_thread->join();
  delete server_thread;
}

}
}
