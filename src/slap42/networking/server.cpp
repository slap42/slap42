#include "server.hpp"

#include <cstdio>
#include <thread>
#include <unordered_map>
#include <enet/enet.h>
#include <glm/glm.hpp>
#include "hud_panels/server_panel.hpp"
#include "networking/message_types.hpp"
#include "networking/message_serializer.hpp"

namespace Slap42 {
namespace Server {

static bool server_running = false;
static std::thread* server_thread;

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
          snprintf(buf, sizeof(buf), "%x:%u", evt.peer->address.host, evt.peer->address.port);
          ServerPanel::OnPlayerJoin(buf);

          OnPlayerJoinMessage msg { evt.peer->address.host, evt.peer->address.port };
          BroadcastSerializedMessage(server, msg, evt.peer);

          ENetPeer* current_peer;
          for (current_peer = server->peers; current_peer < &server->peers[server->peerCount]; ++current_peer) {
            if (current_peer->state != ENET_PEER_STATE_CONNECTED) {
              continue;
            }
            if (current_peer == evt.peer) {
              continue;
            }

            OnPlayerJoinMessage msg2 { current_peer->address.host, current_peer->address.port };
            SendSerializedMessage(evt.peer, msg2);
          }
          break;
        }

        case ENET_EVENT_TYPE_DISCONNECT: {
          printf("[SERVER] A client has disconnected: %x:%u\n", evt.peer->address.host, evt.peer->address.port);

          char buf[128] { };
          snprintf(buf, sizeof(buf), "%x:%u", evt.peer->address.host, evt.peer->address.port);
          ServerPanel::OnPlayerLeave(buf);

          OnPlayerLeaveMessage msg { evt.peer->address.host, evt.peer->address.port };
          BroadcastSerializedMessage(server, msg, evt.peer);

          break;
        }

        case ENET_EVENT_TYPE_RECEIVE: {
          bytepack::binary_stream stream(bytepack::buffer_view(evt.packet->data, evt.packet->dataLength));
          MessageType type;
          stream.read(type);

          switch (type) {
            case MessageType::kPositionUpdate: {
              PositionUpdateMessage msg { };
              msg.deserialize(stream);
              msg.host = evt.peer->address.host;
              msg.port = evt.peer->address.port;
              BroadcastSerializedMessage(server, msg, evt.peer);
              break;
            }
            
            default:
              break;
          }
        }
          
        case ENET_EVENT_TYPE_NONE:
          break;
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
