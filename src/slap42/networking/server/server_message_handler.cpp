#include "server_message_handler.hpp"

#include <bytepack/bytepack.hpp>
#include "networking/disconnect_reasons.hpp"
#include "networking/message_serializer.hpp"
#include "networking/message_types.hpp"
#include "server_data.hpp"

namespace Slap42 {
namespace Server {

void OnClientConnect(ENetEvent& evt) {
  // Store ID in peer user data
  // We can't use connectID as this is set to zero on disconnect
  peer_id current_id = 0;
  while (peer_data.find(current_id) != peer_data.end()) {
    ++current_id;
  }
  evt.peer->data = (void*)(uint64_t)current_id;
  
  auto new_peer = std::make_shared<PeerData>();
  new_peer->peer = evt.peer;
  peer_data.emplace(current_id, new_peer);

  // Tell the new peer server metadata such as the total capacity and the player's own ID
  {
    ServerInfoMessage msg {
      .id = current_id,
      .capacity = capacity,
      .seed = seed,
    };
    SendSerializedMessage(evt.peer, msg);
  }
  
  // Tell all current peers that a new peer has connected
  {
    PlayerJoinMessage msg { current_id };
    BroadcastSerializedMessage(server, msg, evt.peer);
  }
  
  // Tell the new peer about all the current peers
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
      fprintf(stderr, "[SERVER] Unrecognized message type received: %u\n", (unsigned int)type);
      break;
    }
  }
}

}
}
