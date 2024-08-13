#include "server.hpp"

#include <cstdio>
#include <thread>
#include <enet/enet.h>

namespace Slap42 {

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
        case ENET_EVENT_TYPE_CONNECT:
          printf("[SERVER] A client has connected: %x:%u\n", evt.peer->address.host, evt.peer->address.port);
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          printf("[SERVER] A client has disconnected: %x:%u\n", evt.peer->address.host, evt.peer->address.port);
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          printf("[SERVER] Recieved packet: \n\tPeer: %s\n\tLength: %zu\n\tData: %s\n\tChannel: %u\n",
            (char*)evt.peer->data, evt.packet->dataLength, (char*)evt.packet->data, evt.channelID);
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

}
