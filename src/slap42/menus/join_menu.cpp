#include "join_menu.hpp"

#include <cstdio>
#include <sstream>
#include <enet/enet.h>
#include <imgui.h>
#include "join_error_menu.hpp"
#include "networking/client.hpp"
#include "menus/menu_state_machine.hpp"
#include "menus/join_async_menu.hpp"

namespace Slap42 {
namespace JoinMenu {

static bool TryParseHostAndPort(std::string buf, std::string& host, uint16_t& port) {
  size_t colon_pos = buf.find_first_of(':');

  if (colon_pos != std::string::npos) {
    // Colon found, try to parse host and port
    host = buf.substr(0, colon_pos);
    
    std::string port_str = buf.substr(colon_pos + 1);
    for (char c : port_str) {
      if (!isdigit(c)) {
        std::stringstream ss;
        ss << port_str << " is not a valid port number";
        JoinErrorMenu::SetErrorMessage(ss.str().c_str());
        return false;
      }
    }

    port = std::stoi(port_str);
    return true;
  }
  else {
    // No colon, use default port
    host = buf;
    port = 6969;
    return true;
  }
}

void Render() {
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Join Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

  static char buf[256] { "localhost:6969" };

  if (ImGui::InputText("Server Address", buf, sizeof(buf), ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::Button("Connect")) {
    std::string host;
    uint16_t port;
    if (TryParseHostAndPort(buf, host, port)) {
      JoinAsyncMenu::Reset(host.c_str(), port);
      MenuStateMachine::SetState(MenuState::kJoinAsyncMenu);
    }
    else {
      MenuStateMachine::SetState(MenuState::kJoinErrorMenu);
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
