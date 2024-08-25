#include "connected_players_menu.hpp"

#include <imgui.h>
#include "menu_state_machine.hpp"
#include "networking/client/client.hpp"
#include "networking/server/server.hpp"
#include "window/controls.hpp"

namespace Slap42 {
namespace ConnectedPlayersMenu {

static peer_id id;
static int capacity;

void Render() {
  static const bool kIsServerOwner = Server::GetState() == ServerState::kRunning;
  
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Connected Players", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
  
  ImGui::Text("Server Capacity: %d/%d", Client::GetPeerData()->size() + 1, capacity);
  ImGui::Spacing();
  
  ImGui::Text("Connected Players:");
  if (ImGui::BeginTable("ConnectedPlayersTable", 2)) {
    
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Player %d (You)", (int)id);
    
    for (auto& peer : *Client::GetPeerData()) {
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("Player %d", peer.first);
      
      if (kIsServerOwner) {
        ImGui::TableSetColumnIndex(1);
        if (ImGui::Button("Kick")) {
          Client::SendKickPlayer(peer.first);
        }
      }
    }
    ImGui::EndTable();
  }
  
  ImGui::Spacing();
  if (ImGui::Button("Back") || Controls::IsButtonPressed(Button::kOpenMainMenu)) {
    MenuStateMachine::SetState(MenuState::kPauseMenu);
  }

  ImGui::End();
}

void SetIdAndCapacity(peer_id i, int c) {
  id = i;
  capacity = c;
}

}
}

