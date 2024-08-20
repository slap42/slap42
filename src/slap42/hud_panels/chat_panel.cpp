#include "chat_panel.hpp"

#include <deque>
#include <imgui.h>
#include "networking/client.hpp"

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
  if (!ImGui::GetIO().WantCaptureKeyboard && ImGui::IsKeyPressed(ImGuiKey_T)) {
    open = !open;
  }
  if (!open) return;
  if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
    open = false;
    return;
  }
  
  ImGui::SetNextWindowSize({ 640, 480 }, ImGuiCond_Always);
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ 0, display_size.y }, ImGuiCond_Always, { 0.0f, 1.0f });
  ImGui::Begin("Chat", &open, ImGuiWindowFlags_AlwaysAutoResize);
 
  for (auto& msg : chat_messages) {
    ImGui::Text("%s", msg.c_str());
  }

  auto content_region = ImGui::GetContentRegionAvail();
  ImGui::BeginChild("Messages", { content_region.x, content_region.y - 25 });
  ImGui::EndChild();

  if (ImGui::IsWindowAppearing()) {
    ImGui::SetKeyboardFocusHere();
  }
  static char buf[256] { };
  if (ImGui::InputText("Chat Message", buf, sizeof(buf), ImGuiInputTextFlags_EnterReturnsTrue)) {
    Client::ClientSendChatMessage(buf);
    std::memset(buf, 0, sizeof(buf));
    ImGui::SetKeyboardFocusHere(-1);
  }

  ImGui::End();
}

}
}
