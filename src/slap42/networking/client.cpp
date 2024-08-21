#include "client.hpp"

#include <cstdio>
#include <sstream>
#include <enet/enet.h>
#include <glm/gtc/type_ptr.hpp>
#include "hud_panels/chat_panel.hpp"
#include "menus/join_error_menu.hpp"
#include "networking/message_types.hpp"
#include "networking/message_serializer.hpp"
#include "networking/peer_data.hpp"
#include "utils/hash.hpp"

namespace Slap42 {
namespace Client {

static ENetHost* client = nullptr;
static ENetPeer* peer = nullptr;

static std::unordered_map<peer_id, std::shared_ptr<PeerData>> peer_data;

std::unordered_map<peer_id, std::shared_ptr<PeerData>>* GetPeerData() {
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
    fprintf(stderr, "[CLIENT] ClientDisconnect called when client is not connected\n");
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
            PlayerPositionUpdateMessage msg { };
            msg.deserialize(stream);
            // printf("[CLIENT] Player moved to: (%.2f, %.2f, %.2f) (%.2f, %.2f)\n", msg.pos.x, msg.pos.y, msg.pos.z, msg.rot.x, msg.rot.y);
            peer_data.at(msg.id)->pos = msg.pos;
            peer_data.at(msg.id)->rot = msg.rot;
            break;
          }
            
          case MessageType::kPlayerJoin: {
            PlayerJoinMessage msg { };
            msg.deserialize(stream);

            std::stringstream ss;
            ss << "Player " << (int)msg.id << " joined the game";
            ChatPanel::AddChatMessage(ss.str());

            auto new_peer_data = std::make_shared<PeerData>();
            new_peer_data->pos = msg.pos;
            new_peer_data->rot = msg.rot;
            peer_data.emplace(msg.id, new_peer_data);
            break;
          }
            
          case MessageType::kPlayerLeave: {
            PlayerLeaveMessage msg { };
            msg.deserialize(stream);

            std::stringstream ss;
            ss << "Player " << (int)msg.id << " left the game";
            ChatPanel::AddChatMessage(ss.str());

            peer_data.erase(msg.id);
            break;
          }

          case MessageType::kChatMessage: {
            ChatMessageMessage msg { };
            msg.deserialize(stream);

            std::stringstream ss;
            ss << "[Player " << (int)msg.id << "] " << msg.msg_buf;

            ChatPanel::AddChatMessage(ss.str());
            break;
          }
            
          default:
            printf("[CLIENT] Unhandled message type received\n");
            break;
        }
        break;
      }
      default:
        break;
    }
  }
}

void ClientSendPositionUpdate(const glm::vec3& pos, const glm::vec2& rot) {
  PlayerPositionUpdateMessage pm { .pos = pos, .rot = rot };
  SendSerializedMessage(peer, pm);
}

void ClientSendChatMessage(const std::string& msg) {
  if (msg.length() > 255) {
    fprintf(stderr, "[CLIENT] Error - Can't send a message of length > 255 \"%s\"\n", msg.c_str());
  }
  ChatMessageMessage cm { };
  strcpy(cm.msg_buf, msg.c_str());
  SendSerializedMessage(peer, cm);
}

}
}
