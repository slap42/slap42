#include "join_async_menu.hpp"

#include <string>
#include <thread>
#include <mutex>
#include <imgui.h>
#include "networking/client.hpp"
#include "menus/menu_state_machine.hpp"
#include "window/controls.hpp"

namespace Slap42 {
namespace JoinAsyncMenu {

static bool ready = false;
static bool done = false;
static std::mutex done_mutex;
static bool success = false;
static std::string host_url;
static std::thread* thread = nullptr;

void Reset(const char* host) {
  ready = true;
  done = false;
  success = false;
  host_url = host;
  thread = nullptr;
}

static void DoThread() {
  success = Client::ClientConnect(host_url.c_str(), 6969);
  std::scoped_lock sl(done_mutex);
  done = true;
}

void Render() {
  if (ready) {
    thread = new std::thread(DoThread);
    ready = false;
  }
  {
    std::scoped_lock sl(done_mutex);
    if (done) {
      thread->join();
      delete thread;

      if (success) {
        MenuStateMachine::SetState(MenuState::kNone);
      }
      else {
        MenuStateMachine::SetState(MenuState::kJoinErrorMenu);
      }
    }
  }

  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowPos({ display_size.x * 0.5f, display_size.y * 0.5f }, ImGuiCond_Always, { 0.5f, 0.5f });

  ImGui::Begin("Join Async", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

  ImGui::Text("Trying to join the game...");

  if (ImGui::Button("Back")) {
    Client::InterruptConnectAttempt();
    MenuStateMachine::SetState(MenuState::kJoinMenu);
  }

  ImGui::End();
}

}
}
