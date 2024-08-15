#include "server.hpp"

#include <cstdio>
#include <thread>
#include <unordered_map>
#include <enet/enet.h>
#include <glm/glm.hpp>
#include "hud_panels/server_panel.hpp"
#include "networking/message_types.hpp"

namespace Slap42 {
namespace Server {

static bool server_running = false;
static std::thread* server_thread;

static std::unordered_map<unsigned short, ENetPeer*> connected_peers;

static void RunServer() {
  ENetAddress address { };
  address.host = ENET_HOST_ANY;
  address.port = 6969;

  ENetHost* server = enet_host_create(&address, 32, 1, 0, 0);
  if (!server) {
    fprintf(stderr, "[SERVER] Failed to start server: enet_host_create failed\n");
    return;
  }

  ENetEvent evt;
  while (server_running) {
    while (enet_host_service(server, &evt, 0) > 0) {
      
      switch (evt.type) {
        case ENET_EVENT_TYPE_CONNECT: {
          printf("[SERVER] A client has connected: %x:%u\n", evt.peer->address.host, evt.peer->address.port);
          char buf[128] { };
          sprintf(buf, "%x:%u", evt.peer->address.host, evt.peer->address.port);
          connected_peers.emplace(evt.peer->address.port, evt.peer);
          ServerPanel::OnPlayerJoin(buf);
          break;
        }

        case ENET_EVENT_TYPE_DISCONNECT: {
          printf("[SERVER] A client has disconnected: %x:%u\n", evt.peer->address.host, evt.peer->address.port);
          char buf[128] { };
          sprintf(buf, "%x:%u", evt.peer->address.host, evt.peer->address.port);
          connected_peers.erase(evt.peer->address.port);
          ServerPanel::OnPlayerLeave(buf);
          break;
        }

        case ENET_EVENT_TYPE_RECEIVE: {
          bytepack::binary_stream stream(bytepack::buffer_view(evt.packet->data, evt.packet->dataLength));
          MessageType type;
          stream.read(type);

          switch (type) {
            case MessageType::kPositionUpdate: {
              PositionUpdateMessage pm { };
              pm.deserialize(stream);
              printf("Player moved to: (%.2f, %.2f, %.2f) (%.2f, %.2f)\n", pm.pos.x, pm.pos.y, pm.pos.z, pm.rot.x, pm.rot.y); 
              break;
            }
          }

        }
      }
    }
  }

  enet_host_destroy(server);
}

void StartServer() {
  if (server_running) return;
  printf("[SERVER] Starting Server\n");
  server_running = true;
  server_thread = new std::thread(RunServer);
}

void StopServer() {
  if (!server_running) return;
  server_running = false;
  server_thread->join();
  printf("[SERVER] Stopped Server\n");
  delete server_thread;
}


bool IsServerRunning() {
  return server_running;
}

}
}
