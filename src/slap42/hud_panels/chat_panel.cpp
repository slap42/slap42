#include "chat_panel.hpp"

#include <deque>
#include <imgui.h>

namespace Slap42 {
namespace ChatPanel {

static std::deque<std::string> chat_messages;

void AddChatMessage(const std::string& msg) {
  const size_t kMaxMessages = 100;
  chat_messages.push_back(msg);
  while (chat_messages.size() > kMaxMessages) {
    chat_messages.pop_front();
  }
}

void Render() {
  static bool open = false;
  if (ImGui::IsKeyPressed(ImGuiKey_T)) {
    open = !open;
  }
  if (!open) return;
  
  ImGui::SetNextWindowSize({ 640, 480 }, ImGuiCond_Always);
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ 0, display_size.y }, ImGuiCond_Always, { 0.0f, 1.0f });
  ImGui::Begin("Chat", &open, ImGuiWindowFlags_AlwaysAutoResize);
 
  for (auto& msg : chat_messages) {
    ImGui::Text("%s", msg.c_str());
  }

  ImGui::End();
}

}
}
