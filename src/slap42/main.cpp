#include <cstdio>

#include <enet/enet.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include "graphics/camera.hpp"
#include "graphics/meshes/entity_mesh.hpp"
#include "graphics/shaders/entity_shader.hpp"
#include "hud_panels/chat_panel.hpp"
#include "hud_panels/server_panel.hpp"
#include "level/level.hpp"
#include "menus/menu_state_machine.hpp"
#include "networking/client.hpp"
#include "networking/server.hpp"
#include "window/window.hpp"

int main() {
  using namespace Slap42;

  atexit(enet_deinitialize);
  if (enet_initialize() != 0) {
    fprintf(stderr, "enet_initialize failed\n");
  }

  Window::CreateWindow();

  Level level;

  EntityShader entity_shader;

  float vertices[] = {
    -1.0f, -1.0f, -1.0f, 0.5f, 0.2f,
     1.0f, -1.0f, -1.0f, 0.5f, 0.2f,
     1.0f,  1.0f, -1.0f, 0.5f, 0.2f,
    -1.0f,  1.0f, -1.0f, 0.5f, 0.2f,
    -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
  };
  unsigned short indices[] = {
    0, 2, 1,
    0, 3, 2,
    4, 5, 6,
    4, 6, 7,
    2, 3, 6,
    3, 7, 6,
    0, 1, 4,
    1, 5, 4,
    1, 2, 5,
    5, 2, 6,
    0, 4, 3,
    4, 7, 3,
  };
  EntityMesh entity_mesh(vertices, sizeof(vertices), indices, sizeof(indices));

  Camera camera(&entity_shader, level.GetShader());

  while (Window::IsWindowOpen()) {
    Client::ClientPollMessages();
    Window::PollWindowEvents();
    
    if (MenuStateMachine::GetState() == MenuState::kNone) {
      if (!ImGui::GetIO().WantCaptureKeyboard) {
        camera.Update();
      }
      level.Update(camera.GetPosition());
    }

    Window::ClearWindow();

    level.Render();

    entity_shader.Bind();

    auto peers = Client::GetPeerData();
    for (auto& peer : *peers) {
      const glm::vec3 kXAxis(1.0f, 0.0f, 0.0f);
      const glm::vec3 kYAxis(0.0f, 1.0f, 0.0f);
      glm::mat4 transform = glm::translate(glm::mat4(1.0f), { -peer.second->pos.x, -peer.second->pos.y, -peer.second->pos.z });
      transform = glm::rotate(transform, -peer.second->rot.y, kYAxis);
      transform = glm::rotate(transform, -peer.second->rot.x, kXAxis);
      entity_shader.SetTransform(transform);
      entity_mesh.Render();
    }

    Window::ImguiBegin();
    // ImGui::ShowDemoWindow();
    MenuStateMachine::Render();
    
    if (MenuStateMachine::GetState() == MenuState::kNone) {
      ServerPanel::Render();
      ChatPanel::Render();
    }

    Window::ImguiEnd();

    Window::SwapBuffers();
  }

  Client::ClientDisconnect();
  Server::StopServer();

  Window::DestroyWindow();
}

#if defined(_WIN32) && defined(NDEBUG)
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
  return main();
}
#endif
