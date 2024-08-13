#include <cstdio>

#include <enet/enet.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "graphics/camera.hpp"
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
  GLFWwindow* window = glfwCreateWindow(1280, 720, "Test", nullptr, nullptr);
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

  Camera camera(window, level.GetShader());
  glfwSetWindowUserPointer(window, &camera);
  auto on_resize = [](GLFWwindow* window, int width, int height) {
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
    ClientUpdate();
    glfwPollEvents();

    if (MenuStateMachine::GetState() == MenuState::kNone) {
      camera.Update();
      level.Update(camera.GetPosition());
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    level.Render();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // ImGui::ShowDemoWindow();
    MenuStateMachine::Render();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  ClientDisconnect();
  StopServer();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}
