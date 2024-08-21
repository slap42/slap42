#include "camera.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <imgui.h>
#include "networking/client.hpp"
#include "window/controls.hpp"
#include "window/window.hpp"

namespace Slap42 {
namespace Camera {

static glm::vec3 position = glm::vec3(0.0f);
static glm::vec2 rotation = glm::vec2(0.0f);
static glm::mat4 view = glm::mat4(1.0f);
static glm::mat4 projection = glm::mat4(1.0f);

static void CalcViewProjection() {
  glm::mat4 view_projection = projection * view;
  Shader::TerrainShader::SetViewProjection(view_projection);
  Shader::EntityShader::SetViewProjection(view_projection);
}

static void CalcView() {
  const glm::vec3 kXAxis(1.0f, 0.0f, 0.0f);
  const glm::vec3 kYAxis(0.0f, 1.0f, 0.0f);
  
  view = glm::rotate(glm::mat4(1.0f), rotation.x, kXAxis);
  view = glm::rotate(view, rotation.y, kYAxis);
  view = glm::translate(view, position);
  CalcViewProjection();
}

static void OnResize(int width, int height) {
  if (width <= 0 || height <= 0) return;
  projection = glm::perspectiveFov(1.22f, (float)width, float(height), 0.1f, 1000.0f);
  CalcViewProjection();
}


void Create() {
  position = glm::vec3(0.0f, 0.0f, -2.0f);
  CalcView();
  
  // TODO: window::GetSize()
  OnResize(1280, 720);
  
  auto on_resize = [](GLFWwindow* window, int w, int h) {
    int width, height;
    glfwGetFramebufferSize(Window::GetGlfwWindow(), &width, &height);
    OnResize(width, height);
    glViewport(0, 0, width, height);
  };
  glfwSetWindowSizeCallback(Window::GetGlfwWindow(), on_resize);
  on_resize(Window::GetGlfwWindow(), 1280, 720);
}

void Update() {
  static GLFWwindow* window = Window::GetGlfwWindow();
  
  const float kMoveSpeed = 0.5f;
  const float kRotationSpeed = 0.04f;
  const float kMouseSensitivity = 0.005f;
  
  glm::vec3 old_position = position;
  glm::vec2 old_rotation = rotation;
  
  // Rotation
  if (glfwGetKey(window, GLFW_KEY_LEFT)) {
    rotation.y -= kRotationSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
    rotation.y += kRotationSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_UP)) {
    rotation.x -= kRotationSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN)) {
    rotation.x += kRotationSpeed;
  }
  
  // Mouse input
  
  // TODO: move this to an event consumer?
  if (!ImGui::GetIO().WantCaptureKeyboard && ImGui::IsKeyPressed(ImGuiKey_Escape)) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
  if (!ImGui::GetIO().WantCaptureMouse && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
  
  if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
    rotation.y += Controls::GetMouseDeltaX() * kMouseSensitivity;
    rotation.x += Controls::GetMouseDeltaY() * kMouseSensitivity;
  }
  
  if (rotation.x != old_rotation.x) {
    if (rotation.x < -1.57f) rotation.x = -1.57f;
    if (rotation.x >  1.57f) rotation.x =  1.57f;
  }
  
  // Movement
  glm::vec3 dir = glm::vec3(0.0f);
  
  if (glfwGetKey(window, GLFW_KEY_W) && !glfwGetKey(window, GLFW_KEY_S)) {
    dir.z = 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_S) && !glfwGetKey(window, GLFW_KEY_W)) {
    dir.z = -1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_A) && !glfwGetKey(window, GLFW_KEY_D)) {
    dir.x = 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_D) && !glfwGetKey(window, GLFW_KEY_A)) {
    dir.x = -1.0f;
  }
  
  if (dir.x != 0 || dir.z != 0) {
    dir = glm::rotateY(dir, -rotation.y);
    position += glm::normalize(dir) * kMoveSpeed;
  }
  
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
    position.y += kMoveSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE)) {
    position.y -= kMoveSpeed;
  }
  
  if (position != old_position || rotation != old_rotation) {
    CalcView();
    Client::ClientSendPositionUpdate(position, rotation);
  }
}

glm::vec3 GetPosition() {
  return position;
}

}
}
