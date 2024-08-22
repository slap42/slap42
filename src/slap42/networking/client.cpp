#include "client.hpp"

#include <chrono>
#include <cstdio>
#include <sstream>
#include <mutex>
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

static bool interrupt_connect_attempt = false;
static std::mutex interrupt_connect_attempt_mutex;

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
  
  // Time this manually because we want to be able to interrupt it
  ENetEvent evt;
  auto start = std::chrono::steady_clock::now();
  while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < 5) {
    if (enet_host_service(client, &evt, 0) && evt.type == ENET_EVENT_TYPE_CONNECT) {
      printf("[CLIENT] Connection to %s:%u succeeded!\n", hostname, port);
      return true;
    }

    std::scoped_lock sl(interrupt_connect_attempt_mutex);
    if (interrupt_connect_attempt) {
      break;
    }
  }
  
  // Timeout, connection fail
  enet_peer_reset(peer);
  client = nullptr;
  peer = nullptr;
  if (!interrupt_connect_attempt) {
    JoinErrorMenu::SetErrorMessage("[CLIENT] Failed to connect to the server\n");
  }
  std::scoped_lock sl(interrupt_connect_attempt_mutex);
  interrupt_connect_attempt = false;
  return false;
}

void InterruptConnectAttempt() {
  std::scoped_lock sl(interrupt_connect_attempt_mutex);
  interrupt_connect_attempt = true;
}

void ClientDisconnect() {
  if (!peer)  {
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
            ChatPanel::AddChatMessage(255, ss.str());

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
            ChatPanel::AddChatMessage(255, ss.str());

            peer_data.erase(msg.id);
            break;
          }

          case MessageType::kChatMessage: {
            ChatMessageMessage msg { };
            msg.deserialize(stream);

            std::stringstream ss;
            ss << "[Player " << (int)msg.id << "] " << msg.msg_buf;

            ChatPanel::AddChatMessage(msg.id, ss.str());
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
    fprintf(stderr, "[CLIENT] Error - Can't send a chat message of length > 255 \"%s\"\n", msg.c_str());
  }
  ChatMessageMessage cm { };
  strcpy(cm.msg_buf, msg.c_str());
  SendSerializedMessage(peer, cm);
}

}
}
