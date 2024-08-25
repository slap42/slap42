#include "join_menu.hpp"

#include <string>
#include <sstream>
#include <imgui.h>
#include "host_menu.hpp"
#include "host_error_menu.hpp"
#include "menu_state_machine.hpp"
#include "networking/client.hpp"
#include "networking/server.hpp"
#include "utils/hostname_port_validation.hpp"

namespace Slap42 {
namespace HostMenu {

void Render() {
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Host Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

  static char buf[256] { "6969" };

  if (ImGui::InputText("Server Port", buf, sizeof(buf), ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::Button("Start Server")) {
    uint16_t port;
    if (Validation::TryParsePort(buf, port)) {
      Server::StopServer();
      Server::StartServer(port);

      // Spin while waiting for server to either start or fail to start - should be a few milliseconds
      while (Server::GetState() == ServerState::kStopped) {
        void(0);
      }

      if (Server::GetState() == ServerState::kRunning) {
        if (Client::Connect("localhost", port)) {
          MenuStateMachine::SetState(MenuState::kNone);
        }
        else {
          HostErrorMenu::SetErrorMessage("Failed to connect to localhost");
          Server::StopServer();
          MenuStateMachine::SetState(MenuState::kHostErrorMenu);
        }
      }
      else {
        Server::StopServer();
        MenuStateMachine::SetState(MenuState::kHostErrorMenu);
      }
    }
    else {
      MenuStateMachine::SetState(MenuState::kHostErrorMenu);
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
