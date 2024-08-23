#include "host_join_menu.hpp"

#include <imgui.h>
#include "networking/client.hpp"
#include "networking/server.hpp"
#include "menus/menu_state_machine.hpp"
#include "window/controls.hpp"

namespace Slap42 {
namespace HostJoinMenu {

void Render() {
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Host or Join Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

  if (ImGui::Button("Host Game")) {
    Server::StopServer();
    Server::StartServer();

    while (Server::GetState() == ServerState::kStopped) {
      void(0);
    }

    if (Server::GetState() == ServerState::kRunning) {
      MenuStateMachine::SetState(MenuState::kNone);
      Client::ClientConnect("127.0.0.1", 6969);
    }
    else {
      Server::StopServer();
      MenuStateMachine::SetState(MenuState::kHostErrorMenu);
    }

  }

  ImGui::SameLine();
  if (ImGui::Button("Join Game")) {
    MenuStateMachine::SetState(MenuState::kJoinMenu);
  }

  ImGui::End();
}

}
}
