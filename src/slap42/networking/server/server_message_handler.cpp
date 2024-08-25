#include "server_message_handler.hpp"

#include <bytepack/bytepack.hpp>
#include "networking/disconnect_reasons.hpp"
#include "networking/message_serializer.hpp"
#include "networking/message_types.hpp"
#include "server_data.hpp"

namespace Slap42 {
namespace Server {

void OnClientConnect(ENetEvent& evt) {
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
}

void OnClientDisconnect(ENetEvent& evt) {
  printf("[SERVER] A client has disconnected: %x:%u\n", evt.peer->address.host, evt.peer->address.port);
 
  PlayerLeaveMessage msg {
    .id = (peer_id)(uint64_t)evt.peer->data,
    .kicked = peer_data[(peer_id)(uint64_t)evt.peer->data]->kicked,
  };
  BroadcastSerializedMessage(server, msg, evt.peer);
  
  peer_data.erase((peer_id)(uint64_t)evt.peer->data);
}

void OnMessageRecv(ENetEvent& evt) {
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
}

}
}
