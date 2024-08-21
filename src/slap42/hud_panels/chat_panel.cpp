#include "chat_panel.hpp"

#include <algorithm>
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

  for (auto& msg : chat_messages) {
    ImGui::TextWrapped("%s", msg.c_str());
  }

  static bool snap_to_bottom = false;
  if (snap_to_bottom) {
    snap_to_bottom = false;
    ImGui::SetScrollY(999999999999.0f);
  }

  ImGui::EndChild();

  if (ImGui::IsWindowAppearing()) {
    ImGui::SetKeyboardFocusHere();
  }

  ImGui::PushItemWidth(window_size.x - 25);
  static char buf[256] { };
  if (ImGui::InputText("##Chat Message", buf, sizeof(buf), ImGuiInputTextFlags_EnterReturnsTrue)) {
    Client::ClientSendChatMessage(buf);
    std::memset(buf, 0, sizeof(buf));
    ImGui::SetKeyboardFocusHere(-1);
    snap_to_bottom = true;
  }
  ImGui::PopItemWidth();

  ImGui::End();
}

}
}
