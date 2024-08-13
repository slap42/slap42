#include "host_join_menu.hpp"

#include <imgui.h>
#include "networking/client.hpp"
#include "networking/server.hpp"
#include "menus/menu_state_machine.hpp"

namespace Slap42 {
namespace HostJoinMenu {

void Render() {
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Host or Join Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

  if (ImGui::Button("Host Game")) {
    MenuStateMachine::SetState(MenuState::kNone);
    StopServer();
    StartServer();
    ClientConnect("127.0.0.1", 6969);
  }

  ImGui::SameLine();
  if (ImGui::Button("Join Game")) {
    MenuStateMachine::SetState(MenuState::kJoinMenu);
  }

  ImGui::End();
}

}
}
