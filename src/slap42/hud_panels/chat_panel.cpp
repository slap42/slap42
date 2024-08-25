#include "chat_panel.hpp"

#include <algorithm>
#include <deque>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/color_space.hpp>
#include <imgui.h>
#include "networking/client/client.hpp"
#include "window/controls.hpp"
#include "window/window.hpp"

// TODO: I have no idea what is including minwindef.h
#ifdef min
  #undef min
#endif

namespace Slap42 {
namespace ChatPanel {

static std::deque<std::pair<peer_id, std::string>> chat_messages;
static int snap_to_bottom = 0;

void AddChatMessage(peer_id sender, const std::string& msg) {
  const size_t kMaxMessages = 100;
  chat_messages.push_back({ sender, msg });
  while (chat_messages.size() > kMaxMessages) {
    chat_messages.pop_front();
  }
  snap_to_bottom = 2;
}

void Render() {
  static bool open = false;
  static bool was_mouse_grabbed = false;

  // Opening chat menu with T
  if (Controls::IsInPlayerInputState() && Controls::IsButtonPressed(Button::kOpenChatMenu)) {
    was_mouse_grabbed = Controls::GetInputState() == InputState::kMouseGrabbed;
    Controls::SetInputState(InputState::kNonBlockingMenu);
    open = true;
  }

  // Closing chat menu by clicking
  if (open && !ImGui::GetIO().WantCaptureMouse && Controls::IsButtonPressed(Button::kActionPrimary)) {
    Controls::SetInputState(InputState::kMouseGrabbed);
    open = false;
  }

  // Closing chat menu with Esc
  if (open && Controls::IsButtonPressed(Button::kOpenMainMenu)) {
    Controls::SetInputState(was_mouse_grabbed ? InputState::kMouseGrabbed : InputState::kMouseFree);
    open = false;
  }

  if (!open) return;
  
  ImVec2 window_size = {
    std::min(640.0f, ImGui::GetMainViewport()->Size.x / 2.0f),
    std::min(480.0f, ImGui::GetMainViewport()->Size.y / 2.0f)
  };
  ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ 0, display_size.y }, ImGuiCond_Always, { 0.0f, 1.0f });

  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
  ImGui::Begin("Chat", &open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
  ImGui::PopStyleVar();

  auto content_region = ImGui::GetContentRegionAvail();
  ImGui::BeginChild("Messages", { content_region.x, content_region.y - 25 });

  for (const auto& msg : chat_messages) {
    if (msg.first != 255) {
      glm::vec3 hsv = glm::rgbColor(glm::vec3{ ((msg.first + (msg.first % 2 == 0 ? 0 : 4)) * 43) % 360, 0.7f, 1.0f });
      ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(hsv.r * 255.0f, hsv.g * 255.0f, hsv.b * 255.0f, 255.0f));
    }
    ImGui::TextWrapped("%s", msg.second.c_str());
    if (msg.first != 255) {
      ImGui::PopStyleColor();
    }
  }

  // We need 1 extra frame to calculate the size of the child window contents before we can set the scroll
  if (snap_to_bottom == 2 && ImGui::GetScrollY() != ImGui::GetScrollMaxY()) {
    snap_to_bottom = 0;
  }
  if (snap_to_bottom > 0) {
    --snap_to_bottom;
    ImGui::SetScrollHereY(1.0f);
  }

  ImGui::EndChild();

  if (ImGui::IsWindowAppearing()) {
    ImGui::SetKeyboardFocusHere();
  }

  ImGui::PushItemWidth(window_size.x - 25);
  // Max chat message length is 255 characters, don't change this without updating the network stuff too
  static char buf[256] { };
  if (ImGui::InputText("##Chat Message", buf, sizeof(buf), ImGuiInputTextFlags_EnterReturnsTrue)) {
    if (std::strlen(buf) > 0) {
      Client::SendChatMessage(buf);
      std::memset(buf, 0, sizeof(buf));
      snap_to_bottom = 2;
    }

    ImGui::SetKeyboardFocusHere(-1);
  }
  ImGui::PopItemWidth();

  ImGui::End();
}

}
}
