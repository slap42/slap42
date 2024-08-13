#pragma once

#include <mutex>
#include <string>
#include <unordered_set>
#include <imgui.h>
#include "networking/server.hpp"

namespace Slap42 {
namespace ServerPanel {

static std::unordered_set<std::string> connected_players;
static std::mutex connected_players_mutex;

void Render() {
  if (!Server::IsServerRunning()) return;

  static bool open = true;
  if (ImGui::IsKeyPressed(ImGuiKey_F2)) {
    open = !open;
  }
  if (!open) return;
  
  ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Once);
  ImGui::Begin("Server", &open, ImGuiWindowFlags_AlwaysAutoResize);
  {
    std::scoped_lock lock(connected_players_mutex);
    ImGui::Text("Connected players:");
    ImGui::Indent();
    for (const auto& player : connected_players) {
      ImGui::Text("%s", player.c_str());
    }
  }

  ImGui::End();
}

void OnPlayerJoin(const char* username) {
  std::scoped_lock lock(connected_players_mutex);
  connected_players.emplace(username);
}

void OnPlayerLeave(const char* username) {
  std::scoped_lock lock(connected_players_mutex);
  connected_players.erase(username);
}

}
}
