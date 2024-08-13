#include <cstdio>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "graphics/camera.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture_array.hpp"
#include "terrain/chunk.hpp"

int main() {
  using namespace Slap42;

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

  Shader shader;
  shader.Bind();

  const char* kFileNames[] = {
    "res/images/dirt.png",
    "res/images/grass.png",
    "res/images/patchy_grass.png",
    "res/images/stone.png",
  };

  TextureArray textures(kFileNames, sizeof(kFileNames) / sizeof(char*));

  Camera camera(window, &shader);
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

  Chunk chunk1( 0,  0);
  Chunk chunk2(-1,  0);
  Chunk chunk3( 0, -1);
  Chunk chunk4(-1, -1);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    camera.Update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shader.Bind();
    chunk1.Render();
    chunk2.Render();
    chunk3.Render();
    chunk4.Render();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}
