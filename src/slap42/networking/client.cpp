#include "client.hpp"

#include <cstdio>
#include <enet/enet.h>
#include <glm/gtc/type_ptr.hpp>
#include "menus/join_error_menu.hpp"
#include "networking/message_types.hpp"

namespace Slap42 {
namespace Client {

static ENetHost* client = nullptr;
static ENetPeer* peer = nullptr;

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
      case ENET_EVENT_TYPE_RECEIVE:
        printf("[CLIENT] Recieved packet: \n\tPeer: %s\n\tLength: %zu\n\tData: %s\n\tChannel: %u\n",
          (char*)evt.peer->data, evt.packet->dataLength, (char*)evt.packet->data, evt.channelID);
        break;
    }
  }
}

void ClientSendPositionUpdate(const glm::vec3& pos, const glm::vec2& rot) {
  PositionUpdateMessage pm { pos, rot };
  bytepack::binary_stream serialization_stream(sizeof(PositionUpdateMessage) + 1);
  serialization_stream.write(MessageType::kPositionUpdate);
  pm.serialize(serialization_stream);
  bytepack::buffer_view buffer = serialization_stream.data();
  ENetPacket* packet = enet_packet_create(buffer.as<std::uint8_t>(), buffer.size(), ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(peer, 0, packet);
}

}
}
