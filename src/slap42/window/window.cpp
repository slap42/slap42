#include "window.hpp"

#include <fstream>
#include <sstream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "controls.hpp"

namespace Slap42 {
namespace Window {

static GLFWwindow* window;

GLFWwindow* GetGlfwWindow() {
  return window;
}

void Create() {
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

  window = glfwCreateWindow(1280, 720, title.str().c_str(), nullptr, nullptr);
  if (!window) {
    fprintf(stderr, "glfwCreateWindow failed\n");
  }

  glfwMakeContextCurrent(window);
  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    fprintf(stderr, "gladLoadGl failed\n");
  }

  if (glfwRawMouseMotionSupported()) {
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0.2, 0.4, 0.6, 1.0);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.IniFilename = nullptr;
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void Destroy() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

bool IsOpen() {
  return !glfwWindowShouldClose(window);
}

void PollEvents() {
  glfwPollEvents();
  
  Controls::Update();
}

void Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SwapBuffers() {
  glfwSwapBuffers(window);
}

void ImguiBegin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImguiEnd() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int GetFramebufferWidth() {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  return width;
}

int GetFramebufferHeight() {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  return height;
}

}
}


