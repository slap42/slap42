#include "join_error_menu.hpp"

#include <string>
#include <imgui.h>
#include "menus/menu_state_machine.hpp"

namespace Slap42 {
namespace JoinErrorMenu {

static std::string message;

void SetErrorMessage(const char* msg) {
  message = msg;
}

void Render() {
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Join Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

  ImGui::Text("%s", message.c_str());

  if (ImGui::Button("Back")) {
    MenuStateMachine::SetState(MenuState::kJoinMenu);
  }

  ImGui::End();
}

}
}
