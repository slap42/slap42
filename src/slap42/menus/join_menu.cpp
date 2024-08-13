#include "join_menu.hpp"

#include <cstdio>
#include <enet/enet.h>
#include <imgui.h>
#include "join_error_menu.hpp"
#include "networking/client.hpp"
#include "menus/menu_state_machine.hpp"

namespace Slap42 {
namespace JoinMenu {

void Render() {
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Join Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

  static char buf[256] { "127.0.0.1" };
  ImGui::InputText("Server URL", buf, sizeof(buf));

  if (ImGui::Button("Connect")) {
    if (Client::ClientConnect(buf, 6969)) {
      MenuStateMachine::SetState(MenuState::kNone);
    }
    else {
      MenuStateMachine::SetState(MenuState::kJoinErrorMenu);
    }
  }

  networking_end:
  ImGui::SameLine();
  if (ImGui::Button("Back")) {
    MenuStateMachine::SetState(MenuState::kHostJoinMenu);
  }

  ImGui::End();
}

}
}
