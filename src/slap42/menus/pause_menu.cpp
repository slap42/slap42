#include "pause_menu.hpp"

#include <imgui.h>
#include "level/level.hpp"
#include "graphics/camera.hpp"
#include "menus/menu_state_machine.hpp"
#include "window/controls.hpp"

namespace Slap42 {
namespace PauseMenu {

void Render() {
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Pause Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

  int render_distance = Level::GetRenderDistance();
  if (ImGui::SliderInt("Render Distance", &render_distance, 5, 30)) {
    Level::SetRenderDistance(render_distance);
  }

  float fov = Camera::GetFov();
  if (ImGui::SliderFloat("FOV", &fov, 1.0f, 2.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp)) {
    Camera::SetFov(fov);
  }

  if (ImGui::Button("Back")) {
    MenuStateMachine::SetState(MenuState::kNone);
  }

  ImGui::End();
}

}
}
