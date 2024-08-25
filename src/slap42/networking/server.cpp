#include "server.hpp"

#include <cstdio>
#include <thread>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <enet/enet.h>
#include <glm/glm.hpp>
#include "disconnect_reasons.hpp"
#include "menus/host_error_menu.hpp"
#include "networking/message_types.hpp"
#include "networking/message_serializer.hpp"
#include "networking/peer_data.hpp"
#include "utils/hash.hpp"

namespace Slap42 {
namespace Server {

static std::mutex server_state_mutex;
static ServerState state = ServerState::kStopped;

static std::thread* server_thread;
static std::unordered_map<peer_id, std::shared_ptr<PeerData>> peer_data;

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

  ENetHost* server = enet_host_create(&address, 32, 1, 0, 0);
  if (!server) {
    HostErrorMenu::SetErrorMessage("[SERVER] Failed to start server: enet_host_create failed.\nThe chosen port might already be in use by another application.");
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
          printf("[SERVER] A client has connected: %x:%u\n", evt.peer->address.host, evt.peer->address.port);
  
          // Store ID in peer user data
          // We can't use connectID as this is set to zero on disconnect
          peer_id current_id = 0;
          while (peer_data.find(current_id) != peer_data.end()) {
            ++current_id;
          }
          evt.peer->data = (void*)(uint64_t)current_id;

          auto new_peer = std::make_shared<PeerData>();
          new_peer->peer = evt.peer;
          peer_data.emplace((peer_id)(uint64_t)evt.peer->data, new_peer);

          {
            PlayerJoinMessage msg { (peer_id)(uint64_t)evt.peer->data };
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

            peer_id id = (peer_id)(uint64_t)current_peer->data;
            PlayerJoinMessage msg {
              .id = id,
              .pos = peer_data[id]->pos,
              .rot = peer_data[id]->rot,
            };
            SendSerializedMessage(evt.peer, msg);
          }
          break;
        }

        case ENET_EVENT_TYPE_DISCONNECT: {
          printf("[SERVER] A client has disconnected: %x:%u\n", evt.peer->address.host, evt.peer->address.port);

          PlayerLeaveMessage msg {
            .id = (peer_id)(uint64_t)evt.peer->data,
            .kicked = peer_data[(peer_id)(uint64_t)evt.peer->data]->kicked,
          };
          BroadcastSerializedMessage(server, msg, evt.peer);
          
          peer_data.erase((peer_id)(uint64_t)evt.peer->data);
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
              msg.id = (peer_id)(uint64_t)evt.peer->data;
              peer_data[msg.id]->pos = msg.pos;
              peer_data[msg.id]->rot = msg.rot;
              BroadcastSerializedMessage(server, msg, evt.peer);
              break;
            }

            case MessageType::kChatMessage: {
              ChatMessageMessage msg { };
              msg.deserialize(stream);
              msg.id = (peer_id)(uint64_t)evt.peer->data;
              BroadcastSerializedMessage(server, msg);
              break;
            }
              
            case MessageType::kKickPlayer: {
              KickPlayerMessage msg { };
              msg.deserialize(stream);
              ENetPeer* peer = peer_data[msg.id]->peer;
              peer_data[msg.id]->kicked = true;
              enet_peer_disconnect(peer, (int)DisconnectReason::kClientKicked);
              break;
            }
            
            default: {
              printf("[SERVER] Unrecognized message type received: %u\n", (unsigned int)type);
              break;
            }
          }
  
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
  printf("[SERVER] Starting Server\n");
  server_thread = new std::thread(RunServer, port);
}

void StopServer() {
  if (GetState() == ServerState::kStopped) return;
  SetState(ServerState::kStopped);
  server_thread->join();
  printf("[SERVER] Stopped Server\n");
  delete server_thread;
}

}
}
