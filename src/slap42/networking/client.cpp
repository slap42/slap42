#include "client.hpp"

#include <cstdio>
#include <enet/enet.h>
#include <glm/gtc/type_ptr.hpp>
#include "menus/join_error_menu.hpp"
#include "networking/message_types.hpp"
#include "networking/message_serializer.hpp"
#include "networking/peer_data.hpp"
#include "utils/hash.hpp"

namespace Slap42 {
namespace Client {

static ENetHost* client = nullptr;
static ENetPeer* peer = nullptr;

static std::unordered_map<uint64_t, std::shared_ptr<PeerData>> peer_data;

std::unordered_map<uint64_t, std::shared_ptr<PeerData>>* GetPeerData() {
  return &peer_data;
}

bool ClientConnect(const char* hostname, uint16_t port) {
  if (peer)  {
    fprintf(stderr, "[CLIENT] ClientConnect called when client is already connected");
    return false;
  }

  client = enet_host_create(nullptr, 1, 1, 0, 0);
  if (!client) {
    JoinErrorMenu::SetErrorMessage("[CLIENT] enet_host_create failed\n");
    return false;
  }
  
  ENetAddress address { };
  // TODO: Parse port from url input
  address.port = port;
  enet_address_set_host(&address, hostname);
  
  peer = enet_host_connect(client, &address, 1, 0);
  if (!peer) {
    client = nullptr;
    JoinErrorMenu::SetErrorMessage("[CLIENT] enet_host_connect failed\n");
    return false;
  }
  
  ENetEvent evt;
  if (enet_host_service(client, &evt, 5000) && evt.type == ENET_EVENT_TYPE_CONNECT) {
    printf("[CLIENT] Connection to %s:%u succeeded!\n", hostname, port);
    return true;
  }
  else {
    enet_peer_reset(peer);
    client = nullptr;
    peer = nullptr;
    JoinErrorMenu::SetErrorMessage("[CLIENT] enet_host_service failed: Connection timed out waiting for connection to the server\n");
    return false;
  }
}

void ClientDisconnect() {
  if (!peer)  {
    fprintf(stderr, "[CLIENT] ClientDisconnect called when client is not connected");
    return;
  }

  ENetEvent evt;
  enet_peer_disconnect(peer, 0);
  while (enet_host_service(client, &evt, 5000) > 0) {
    switch (evt.type) {
      case ENET_EVENT_TYPE_RECEIVE:
        enet_packet_destroy(evt.packet);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        printf("[CLIENT] Disconnect succeeded\n");
        goto break_loop;
      default:
        break;
    }
  }
  break_loop:
  void(0);
}

void ClientPollMessages() {
  if (!peer) return;

  ENetEvent evt;
  while (enet_host_service(client, &evt, 0) > 0) {
    switch (evt.type) {
      case ENET_EVENT_TYPE_RECEIVE: {
        bytepack::binary_stream stream(bytepack::buffer_view(evt.packet->data, evt.packet->dataLength));
        MessageType type;
        stream.read(type);
        
        switch (type) {
          case MessageType::kPositionUpdate: {
            PositionUpdateMessage msg { };
            msg.deserialize(stream);
            // printf("[CLIENT] Player moved to: (%.2f, %.2f, %.2f) (%.2f, %.2f)\n", msg.pos.x, msg.pos.y, msg.pos.z, msg.rot.x, msg.rot.y);
            peer_data.at(FakeHash(msg.host, msg.port))->pos = msg.pos;
            peer_data.at(FakeHash(msg.host, msg.port))->rot = msg.rot;
            break;
          }
            
          case MessageType::kOnPlayerJoin: {
            OnPlayerJoinMessage msg { };
            msg.deserialize(stream);
            printf("[CLIENT] A player has joined the game: %x:%u\n", msg.host, msg.port);
            peer_data.emplace(FakeHash(msg.host, msg.port), std::make_shared<PeerData>());
            break;
          }
            
          case MessageType::kOnPlayerLeave: {
            OnPlayerLeaveMessage msg { };
            msg.deserialize(stream);
            printf("[CLIENT] A player has left the game: %x:%u\n", msg.host, msg.port);
            peer_data.erase(FakeHash(msg.host, msg.port));
            break;
          }
            
          default:
            printf("[CLIENT] Unhandled message type recieved\n");
            break;
        }
      }
      default:
        break;
    }
  }
}

void ClientSendPositionUpdate(const glm::vec3& pos, const glm::vec2& rot) {
  PositionUpdateMessage pm { pos, rot };
  SendSerializedMessage(peer, pm);
}

}
}
