#pragma once

#include <bytepack/bytepack.hpp>
#include <enet/enet.h>
#include "message_types.hpp"

namespace Slap42 {

template <typename T>
void SendSerializedMessage(ENetPeer* peer, const T& t) {
  bytepack::binary_stream serialization_stream(sizeof(MessageType) + sizeof(T) + 1);
  serialization_stream.write(t.Type());
  t.serialize(serialization_stream);
  bytepack::buffer_view buffer = serialization_stream.data();
  ENetPacket* packet = enet_packet_create(buffer.as<std::uint8_t>(), buffer.size(), ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(peer, 0, packet);
}

template <typename T>
void BroadcastSerializedMessage(ENetHost* host, const T& t, ENetPeer* to_ignore = nullptr) {
  bytepack::binary_stream serialization_stream(sizeof(MessageType) + sizeof(T) + 1);
  serialization_stream.write(t.Type());
  t.serialize(serialization_stream);
  bytepack::buffer_view buffer = serialization_stream.data();
  ENetPacket* packet = enet_packet_create(buffer.as<std::uint8_t>(), buffer.size(), ENET_PACKET_FLAG_RELIABLE);

  ENetPeer* current_peer;
  for (current_peer = host->peers; current_peer < &host->peers[host->peerCount]; ++current_peer) {
    if (current_peer->state != ENET_PEER_STATE_CONNECTED) {
      continue;
    }
    if (to_ignore && current_peer->data == to_ignore->data) {
      continue;
    }

    enet_peer_send (current_peer, 0, packet);
  }

  if (packet->referenceCount == 0) {
    enet_packet_destroy (packet);
  }
}

}
