#include "client_message_handler.hpp"

#include <memory>
#include <sstream>
#include <bytepack/bytepack.hpp>
#include "client.hpp"
#include "client_data.hpp"
#include "graphics/camera.hpp"
#include "hud_panels/chat_panel.hpp"
#include "level/noise.hpp"
#include "menus/connected_players_menu.hpp"
#include "menus/error_menu.hpp"
#include "menus/menu_state_machine.hpp"
#include "networking/disconnect_reasons.hpp"
#include "networking/message_types.hpp"
#include "networking/peer_data.hpp"

namespace Slap42 {
namespace Client {

void OnServerDisconnect(ENetEvent& evt) {    
  switch ((DisconnectReason)evt.data) {
  case DisconnectReason::kClientKicked:
    ErrorMenu::SetErrorMessage("You were kicked from the server :(");
    break;
  default:
    // If we get here the connection probably timed out, all we know is that the disconnect was not initiated by a player
    ErrorMenu::SetErrorMessage("Disconnected from server");
    break;
  }
  MenuStateMachine::SetState(MenuState::kErrorMenu);        
  Disconnect();
}

void OnMessageRecv(ENetEvent& evt) {
  bytepack::binary_stream stream(bytepack::buffer_view(evt.packet->data, evt.packet->dataLength));
  MessageType type;
  stream.read(type);

  switch (type) {
    case MessageType::kServerInfo: {
      ServerInfoMessage msg { };
      msg.deserialize(stream);
      Noise::SetSeed(msg.seed);
      Camera::SetPosition({ 0.0f, Noise::SampleTerrainHeight(0.0f, 0.0f) - 1.8f, 0.0f});
      ConnectedPlayersMenu::SetIdAndCapacity(msg.id, msg.capacity);
      break;
    }

    case MessageType::kPositionUpdate: {
      PlayerPositionUpdateMessage msg { };
      msg.deserialize(stream);
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
      if (!msg.kicked) {
        ss << "Player " << (int)msg.id << " left the game";
      }
      else {
        ss << "Player " << (int)msg.id << " was kicked";
      }
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
      fprintf(stderr, "[CLIENT] Unhandled message type received\n");
      break;
  }
}

}
}
