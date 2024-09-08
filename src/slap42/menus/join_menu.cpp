#include "join_menu.hpp"

#include <imgui.h>
#include "menus/menu_state_machine.hpp"
#include "menus/join_async_menu.hpp"
#include "utils/hostname_port_validation.hpp"

namespace Slap42 {
namespace JoinMenu {

void Render() {
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Join Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

  static char buf[256] { "127.0.0.1:6969" };

  if (ImGui::InputText("Server Address", buf, sizeof(buf), ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::Button("Connect")) {
    std::string host;
    uint16_t port;
    if (Validation::TryParseHostAndPort(buf, host, port)) {
      JoinAsyncMenu::Reset(host.c_str(), port);
      MenuStateMachine::SetState(MenuState::kJoinAsyncMenu);
    }
    else {
      MenuStateMachine::SetState(MenuState::kErrorMenu);
    }
  }

  ImGui::SameLine();
  if (ImGui::Button("Back")) {
    MenuStateMachine::SetState(MenuState::kHostJoinMenu);
  }

  ImGui::End();
}

}
}
