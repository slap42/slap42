#include <cstdio>
#include <fstream>
#include <sstream>

#include <enet/enet.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "graphics/camera.hpp"
#include "graphics/meshes/entity_mesh.hpp"
#include "graphics/shaders/entity_shader.hpp"
#include "hud_panels/chat_panel.hpp"
#include "hud_panels/server_panel.hpp"
#include "level/level.hpp"
#include "menus/menu_state_machine.hpp"
#include "networking/client.hpp"
#include "networking/server.hpp"

int main() {
  using namespace Slap42;

  atexit(enet_deinitialize);
  if (enet_initialize() != 0) {
    fprintf(stderr, "enet_initialize failed\n");
  }

  if (!glfwInit()) {
    fprintf(stderr, "glfwInit failed\n");
  }
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  std::ifstream version_file("res/version.txt");
  std::stringstream title;
  title << "Slap42 " << version_file.rdbuf();
#ifndef NDEBUG
  title << " Debug";
#endif

  GLFWwindow* window = glfwCreateWindow(1280, 720, title.str().c_str(), nullptr, nullptr);
  if (!window) {
    fprintf(stderr, "glfwCreateWindow failed\n");
  }

  glfwMakeContextCurrent(window);
  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    fprintf(stderr, "gladLoadGl failed\n");
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.IniFilename = nullptr;
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

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

  Camera camera(window, &entity_shader, level.GetShader());
  glfwSetWindowUserPointer(window, &camera);
  auto on_resize = [](GLFWwindow* window, int w, int h) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
    camera->OnResize(width, height);
    glViewport(0, 0, width, height);
  };
  glfwSetWindowSizeCallback(window, on_resize);
  on_resize(window, 1280, 720);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0.2, 0.4, 0.6, 1.0);

  while (!glfwWindowShouldClose(window)) {
    Client::ClientPollMessages();
    glfwPollEvents();

    if (MenuStateMachine::GetState() == MenuState::kNone) {
      camera.Update();
      level.Update(camera.GetPosition());
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // ImGui::ShowDemoWindow();
    MenuStateMachine::Render();
    
    if (MenuStateMachine::GetState() == MenuState::kNone) {
      ServerPanel::Render();
      ChatPanel::Render();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  Client::ClientDisconnect();
  Server::StopServer();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

#if defined(_WIN32) && defined(NDEBUG)
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
  return main();
}
#endif
