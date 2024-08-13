#include "client.hpp"

#include <cstdio>
#include <enet/enet.h>
#include "menus/join_error_menu.hpp"
#include "menus/menu_state_machine.hpp"

namespace Slap42 {

static ENetHost* client = nullptr;
static ENetPeer* peer = nullptr;

void ClientConnect(const char* hostname, uint16_t port) {
  if (peer)  {
    fprintf(stderr, "[CLIENT] ClientConnect called when client is already connected");
    return;
  }

  client = enet_host_create(nullptr, 1, 1, 0, 0);
  if (!client) {
    JoinErrorMenu::SetErrorMessage("[CLIENT] enet_host_create failed\n");
    MenuStateMachine::SetState(MenuState::kJoinErrorMenu);
    return;
  }
  
  ENetAddress address { };
  // TODO: Parse port from url input
  address.port = port;
  enet_address_set_host(&address, hostname);
  
  peer = enet_host_connect(client, &address, 1, 0);
  if (!peer) {
    client = nullptr;
    JoinErrorMenu::SetErrorMessage("[CLIENT] enet_host_connect failed\n");
    MenuStateMachine::SetState(MenuState::kJoinErrorMenu);
    return;
  }
  
  ENetEvent evt;
  if (enet_host_service(client, &evt, 5000) && evt.type == ENET_EVENT_TYPE_CONNECT) {
    printf("[CLIENT] Connection to %s:%u succeeded!\n", hostname, port);
  }
  else {
    enet_peer_reset(peer);
    JoinErrorMenu::SetErrorMessage("[CLIENT] enet_host_service failed: Connection timed out waiting for connection to the server\n");
    MenuStateMachine::SetState(MenuState::kJoinErrorMenu);
    return;
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

void ClientUpdate() {
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

}
