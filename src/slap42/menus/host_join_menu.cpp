#include "host_join_menu.hpp"

#include <imgui.h>

namespace Slap42 {
namespace HostJoinMenu {

void Render(MenuState* state) {
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Host or Join Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

  if (ImGui::Button("Host Game")) {
    *state = MenuState::kNone;
  }

  ImGui::SameLine();
  if (ImGui::Button("Join Game")) {
    *state = MenuState::kJoinMenu;
  }

  ImGui::End();
}

}
}
