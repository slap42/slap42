#include "pause_menu.hpp"

#include <imgui.h>
#include "level/level.hpp"
#include "menus/menu_state_machine.hpp"
#include "window/controls.hpp"

namespace Slap42 {
namespace PauseMenu {

void Render() {
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Pause Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

  ImGui::SliderInt("Render Distance", Level::GetRenderDistancePtr(), 3, 30);

  if (ImGui::Button("Back")) {
    MenuStateMachine::SetState(MenuState::kNone);
  }

  ImGui::End();
}

}
}
