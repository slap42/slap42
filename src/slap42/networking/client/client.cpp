#include "client.hpp"

#include <chrono>
#include <cstdio>
#include <sstream>
#include <mutex>
#include <enet/enet.h>
#include <glm/gtc/type_ptr.hpp>
#include "graphics/camera.hpp"
#include "client_data.hpp"
#include "client_message_handler.hpp"
#include "hud_panels/chat_panel.hpp"
#include "level/level.hpp"
#include "menus/error_menu.hpp"
#include "menus/menu_state_machine.hpp"
#include "networking/disconnect_reasons.hpp"
#include "networking/message_types.hpp"
#include "networking/message_serializer.hpp"
#include "networking/peer_data.hpp"
#include "utils/hash.hpp"

namespace Slap42 {
namespace Client {

static ENetHost* client = nullptr;
static ENetPeer* server = nullptr;

static bool interrupt_connect_attempt = false;
static std::mutex interrupt_connect_attempt_mutex;

std::unordered_map<peer_id, std::shared_ptr<PeerData>>* GetPeerData() {
  return &peer_data;
}

bool Connect(const char* hostname, uint16_t port) {
  if (server)  {
    fprintf(stderr, "[CLIENT] ClientConnect called when client is already connected");
    return false;
  }

  client = enet_host_create(nullptr, 1, 1, 0, 0);
  if (!client) {
    ErrorMenu::SetErrorMessage("[CLIENT] enet_host_create failed\n");
    return false;
  }
  
  ENetAddress address { };
  // TODO: Parse port from url input
  address.port = port;
  enet_address_set_host(&address, hostname);
  
  server = enet_host_connect(client, &address, 1, 0);
  if (!server) {
    client = nullptr;
    ErrorMenu::SetErrorMessage("[CLIENT] enet_host_connect failed\n");
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
  enet_peer_reset(server);
  client = nullptr;
  server = nullptr;
  if (!interrupt_connect_attempt) {
    ErrorMenu::SetErrorMessage("[CLIENT] Failed to connect to the server\n");
  }
  std::scoped_lock sl(interrupt_connect_attempt_mutex);
  interrupt_connect_attempt = false;
  return false;
}

void InterruptConnectAttempt() {
  std::scoped_lock sl(interrupt_connect_attempt_mutex);
  interrupt_connect_attempt = true;
}

void Disconnect() {
  if (!server)  {
    return;
  }

  ENetEvent evt;
  enet_peer_disconnect(server, (int)DisconnectReason::kClientVoluntaryDisconnect);
  // No need to wait here, as the connection will timeout on server side
  while (enet_host_service(client, &evt, 0) > 0) {
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

  // Cleanup game data
  Level::UnloadChunks();
  Camera::ResetPosition();
  peer_data.clear();

  // Destroy server connection
  enet_peer_reset(server);
  server = nullptr;
}

void PollMessages() {
  if (!server) return;

  ENetEvent evt;
  while (enet_host_service(client, &evt, 0) > 0) {
    switch (evt.type) {
      case ENET_EVENT_TYPE_RECEIVE: {
        OnMessageRecv(evt);
        break;
      }
        
      case ENET_EVENT_TYPE_DISCONNECT: {
        OnServerDisconnect(evt);
        break;
      }
      
      default:
        break;
    }
  }
}

void SendPositionUpdate(const glm::vec3& pos, const glm::vec2& rot) {
  PlayerPositionUpdateMessage pm { .pos = pos, .rot = rot };
  SendSerializedMessage(server, pm);
}

void SendChatMessage(const std::string& msg) {
  if (msg.length() > 255) {
    fprintf(stderr, "[CLIENT] Error - Can't send a chat message of length > 255 \"%s\"\n", msg.c_str());
  }
  ChatMessageMessage cm { };
  strcpy(cm.msg_buf, msg.c_str());
  SendSerializedMessage(server, cm);
}

void SendKickPlayer(peer_id id) {
  KickPlayerMessage km { 
    .id = id,
  };
  SendSerializedMessage(server, km);
}

}
}
