#include "join_menu.hpp"

#include <random>
#include <string>
#include <sstream>
#include <imgui.h>
#include <picosha2.h>
#include "host_menu.hpp"
#include "error_menu.hpp"
#include "menu_state_machine.hpp"
#include "networking/client/client.hpp"
#include "networking/server/server.hpp"
#include "utils/hostname_port_validation.hpp"
#include "utils/stoi_wrapper.hpp"

namespace Slap42 {
namespace HostMenu {

static char seed_buf[256] { "" };

void Render() {
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Host Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

  static int capacity = 32;
  ImGui::InputInt("Max Server Capacity", &capacity);
  capacity = std::clamp(capacity, 1, 255);
  
  ImGui::InputText("World Seed", seed_buf, sizeof(seed_buf));

  static char port_buf[256] { "6969" };
  if (ImGui::InputText("Server Port", port_buf, sizeof(port_buf), ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::Button("Start Server")) {
    uint16_t port;
    bool result = Validation::TryParsePort(port_buf, port);
    if (!result) {
      MenuStateMachine::SetState(MenuState::kErrorMenu);
      ImGui::End();
      return;
    }

    Server::StopServer();
    uint32_t seed = 0;
    if (!Validation::TryStou(seed_buf, &seed)) {
      // If the seed cannot be parsed to a number, hash it
      std::string hash_hex_str;
      picosha2::hash256_hex_string(std::string(seed_buf), hash_hex_str);
      std::stringstream ss;
      ss << std::hex << hash_hex_str.substr(0, 8);
      ss >> seed;
    }
    Server::StartServer(port, capacity, seed);

    // Spin while waiting for server to either start or fail to start - should be a few milliseconds
    while (Server::GetState() == ServerState::kStopped) {
      void(0);
    }

    if (Server::GetState() == ServerState::kError) {
      Server::StopServer();
      MenuStateMachine::SetState(MenuState::kErrorMenu);
      ImGui::End();
      return;
    }

    if (Client::Connect("localhost", port)) {
      MenuStateMachine::SetState(MenuState::kNone);
    }
    else {
      ErrorMenu::SetErrorMessage("Failed to connect to localhost");
      Server::StopServer();
      MenuStateMachine::SetState(MenuState::kErrorMenu);
    }
  }

  ImGui::SameLine();
  if (ImGui::Button("Back")) {
    MenuStateMachine::SetState(MenuState::kHostJoinMenu);
  }

  ImGui::End();
}

void GenerateRandomSeed() {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(0, std::numeric_limits<uint32_t>::max());
  std::string s = std::to_string(dist(rng));
  std::strcpy(seed_buf, s.c_str());
}

}
}
