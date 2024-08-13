#include "join_menu.hpp"

#include <imgui.h>

namespace Slap42 {
namespace JoinMenu {

void Render(MenuState* state) {
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Join Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

  static char buf[256];
  ImGui::InputText("Server URL", buf, sizeof(buf));

  if (ImGui::Button("Connect")) {
  }

  ImGui::SameLine();
  if (ImGui::Button("Back")) {
    *state = MenuState::kHostJoinMenu;
  }

  ImGui::End();
}

}
}
