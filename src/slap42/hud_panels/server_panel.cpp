#include <imgui.h>
#include "networking/client.hpp"
#include "networking/server.hpp"

namespace Slap42 {
namespace ServerPanel {

void Render() {
  if (!Server::IsServerRunning()) return;

  static bool open = false;
  if (ImGui::IsKeyPressed(ImGuiKey_F2)) {
    open = !open;
  }
  if (!open) return;
  
  ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Once);
  ImGui::Begin("Server", &open, ImGuiWindowFlags_AlwaysAutoResize);
  
  ImGui::Text("Connected players:");
  ImGui::Indent();
  ImGui::Text("Host player (you)");
  auto* peers = Client::GetPeerData();
  for (const auto& peer : *peers) {
    ImGui::Text("Player with ID %d", peer.first);
  }

  ImGui::End();
}

}
}
