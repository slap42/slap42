#include <chrono>
#include <cstdio>

#include <enet/enet.h>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "graphics/camera.hpp"
#include "graphics/meshes/entity_mesh.hpp"
#include "graphics/shaders/entity_shader.hpp"
#include "hud_panels/chat_panel.hpp"
#include "level/level.hpp"
#include "menus/menu_state_machine.hpp"
#include "networking/client/client.hpp"
#include "networking/server/server.hpp"
#include "window/controls.hpp"
#include "window/window.hpp"

int main() {
  using namespace Slap42;

  atexit(enet_deinitialize);
  atexit(Window::Destroy);
  atexit(Level::Destroy);

  if (enet_initialize() != 0) {
    fprintf(stderr, "enet_initialize failed\n");
  }

  Window::Create();

  Level::Create();

  Shader::EntityShader::Create();

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

  Camera::Create();

  auto last_time = std::chrono::high_resolution_clock::now();

  while (Window::IsOpen()) {
    auto current_time = std::chrono::high_resolution_clock::now();
    float delta = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time).count();
    last_time = current_time;
    
    MenuStateMachine::Update();
    Client::PollMessages();
    Window::PollEvents();

    if (Controls::IsInPlayerInputState() || Controls::GetInputState() == InputState::kNonBlockingMenu) {
      // Update sun position and clear color for a day/night cycle
      // TODO: Get initial sun_animation_time from server and occasionally sync with other players
      static float sun_animation_time = 2.0f; // Start in mid morning
      sun_animation_time += delta * 0.00000027f; // 1 day/night cycle is approx 1 hour
      glm::vec3 sun_dir = glm::vec3(std::sin(sun_animation_time), std::cos(sun_animation_time), 0.0f);
      Shader::TerrainShader::SetSunDirection(sun_dir);
      float sky_brightness = std::clamp(-std::cos(sun_animation_time) + 0.7f, 0.0f, 1.0f);
      glClearColor(0.53f * sky_brightness, 0.81f * sky_brightness, 0.92f * sky_brightness, 1.0f);

      Level::Update(delta);
    }

    if (Controls::IsInPlayerInputState()) {
      Camera::Update(delta);

      if (Controls::IsButtonPressed(Button::kOpenMainMenu)) {
        MenuStateMachine::SetState(MenuState::kPauseMenu);
      }
    }

    Window::Clear();

    Level::Render();

    Shader::EntityShader::Bind();

    auto* peers = Client::GetPeerData();
    for (auto& peer : *peers) {
      const glm::vec3 kXAxis(1.0f, 0.0f, 0.0f);
      const glm::vec3 kYAxis(0.0f, 1.0f, 0.0f);
      glm::mat4 transform = glm::translate(glm::mat4(1.0f), { -peer.second->pos.x, -peer.second->pos.y, -peer.second->pos.z });
      transform = glm::rotate(transform, -peer.second->rot.y, kYAxis);
      transform = glm::rotate(transform, -peer.second->rot.x, kXAxis);
      Shader::EntityShader::SetTransform(transform);
      entity_mesh.Render();
    }

    Window::ImguiBegin();
    // ImGui::ShowDemoWindow();
    MenuStateMachine::Render();
    
    if (MenuStateMachine::GetState() == MenuState::kNone) {
      ChatPanel::Render();
    }

    Window::ImguiEnd();

    Window::SwapBuffers();
  }

  Client::Disconnect();
  Server::StopServer();
}

#if defined(_WIN32) && defined(NDEBUG)
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
  return main();
}
#endif
