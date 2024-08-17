#include "server.hpp"

#include <cstdio>
#include <thread>
#include <unordered_map>
#include <enet/enet.h>
#include <glm/glm.hpp>
#include "networking/message_types.hpp"
#include "networking/message_serializer.hpp"
#include "networking/peer_data.hpp"
#include "utils/hash.hpp"

namespace Slap42 {
namespace Server {

static bool server_running = false;
static std::thread* server_thread;
static std::unordered_map<peer_id, std::shared_ptr<PeerData>> peer_data;
static std::unordered_map<uint64_t, peer_id> peer_ids;

static peer_id GetPeerId(ENetPeer* peer) {
  return peer_ids[FakeHash(peer->address.host, peer->address.port)];
}

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

          peer_id new_peer_id = 0;
          while (peer_data.find(new_peer_id) != peer_data.end()) {
            ++new_peer_id;
          }

          peer_data.emplace(new_peer_id, std::make_shared<PeerData>());
          peer_ids.emplace(FakeHash(evt.peer->address.host, evt.peer->address.port), new_peer_id);

          {
            PlayerJoinMessage msg { new_peer_id };
            BroadcastSerializedMessage(server, msg, evt.peer);
          }

          ENetPeer* current_peer;
          for (current_peer = server->peers; current_peer < &server->peers[server->peerCount]; ++current_peer) {
            if (current_peer->state != ENET_PEER_STATE_CONNECTED) {
              continue;
            }
            if (current_peer == evt.peer) {
              continue;
            }

            peer_id current_peer_id = GetPeerId(current_peer);
            PlayerJoinMessage msg {
              .id = current_peer_id,
              .pos = peer_data[current_peer_id]->pos,
              .rot = peer_data[current_peer_id]->rot,
            };
            SendSerializedMessage(evt.peer, msg);
          }
          break;
        }

        case ENET_EVENT_TYPE_DISCONNECT: {
          printf("[SERVER] A client has disconnected: %x:%u\n", evt.peer->address.host, evt.peer->address.port);

          PlayerLeaveMessage msg { GetPeerId(evt.peer) };
          BroadcastSerializedMessage(server, msg, evt.peer);
          
          peer_data.erase(GetPeerId(evt.peer));
          peer_ids.erase(FakeHash(evt.peer->address.host, evt.peer->address.port));

          break;
        }

        case ENET_EVENT_TYPE_RECEIVE: {
          bytepack::binary_stream stream(bytepack::buffer_view(evt.packet->data, evt.packet->dataLength));
          MessageType type;
          stream.read(type);

          switch (type) {
            case MessageType::kPositionUpdate: {
              PlayerPositionUpdateMessage msg { };
              msg.deserialize(stream);
              msg.id = GetPeerId(evt.peer);
              peer_data[msg.id]->pos = msg.pos;
              peer_data[msg.id]->rot = msg.rot;
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
