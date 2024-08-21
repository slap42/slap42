#include "chat_panel.hpp"

#include <algorithm>
#include <deque>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/color_space.hpp>
#include <imgui.h>
#include "networking/client.hpp"
#include "window/window.hpp"

namespace Slap42 {
namespace ChatPanel {

static std::deque<std::pair<peer_id, std::string>> chat_messages;

void AddChatMessage(peer_id sender, const std::string& msg) {
  const size_t kMaxMessages = 100;
  chat_messages.push_back({ sender, msg });
  while (chat_messages.size() > kMaxMessages) {
    chat_messages.pop_front();
  }
}

void Render() {
  static bool open = false;
  static bool was_cursor_hidden = false;
  if (!open && !ImGui::GetIO().WantCaptureKeyboard && ImGui::IsKeyPressed(ImGuiKey_T)) {
    open = true;
    was_cursor_hidden = glfwGetInputMode(Window::GetGlfwWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
    glfwSetInputMode(Window::GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
  if (!open) return;
  if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
    open = false;
    if (was_cursor_hidden) {
      glfwSetInputMode(Window::GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    return;
  }
  if (glfwGetInputMode(Window::GetGlfwWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
    open = false;
    return;
  }
  
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
  static int snap_to_bottom = 2;
  if (snap_to_bottom < 2) {
    ++snap_to_bottom;
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
    Client::ClientSendChatMessage(buf);
    std::memset(buf, 0, sizeof(buf));
    ImGui::SetKeyboardFocusHere(-1);
    snap_to_bottom = 0;
  }
  ImGui::PopItemWidth();

  ImGui::End();
}

}
}
