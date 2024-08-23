#include "camera.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <imgui.h>
#include "gl_check.hpp"
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
    GL_CHECK(glViewport(0, 0, width, height));
  };
  glfwSetWindowSizeCallback(Window::GetGlfwWindow(), on_resize);
  on_resize(Window::GetGlfwWindow(), 1280, 720);
}

void Update(float delta) {
  if (Controls::IsInputStateInMenu()) {
    return;
  }

  static GLFWwindow* window = Window::GetGlfwWindow();
  
  constexpr float kMoveSpeed = 0.05f;
  constexpr float kRotationSpeed = 0.04f;
  constexpr float kMouseSensitivity = 0.005f;
  
  glm::vec3 old_position = position;
  glm::vec2 old_rotation = rotation;
  
  // Rotation
  if (Controls::IsButtonDown(Button::kRotateLeft)) {
    rotation.y -= kRotationSpeed;
  }
  if (Controls::IsButtonDown(Button::kRotateRight)) {
    rotation.y += kRotationSpeed;
  }
  if (Controls::IsButtonDown(Button::kRotateUp)) {
    rotation.x -= kRotationSpeed;
  }
  if (Controls::IsButtonDown(Button::kRotateDown)) {
    rotation.x += kRotationSpeed;
  }
  
  // Mouse input
  if (Controls::GetInputState() == InputState::kMouseGrabbed) {
    rotation.y += Controls::GetMouseDeltaX() * kMouseSensitivity;
    rotation.x += Controls::GetMouseDeltaY() * kMouseSensitivity;
  }
  
  if (rotation.x != old_rotation.x) {
    if (rotation.x < -1.57f) rotation.x = -1.57f;
    if (rotation.x >  1.57f) rotation.x =  1.57f;
  }

  // Grabbing mouse
  if (Controls::IsButtonPressed(Button::kActionPrimary)) {
    Controls::SetInputState(InputState::kMouseGrabbed);
  }
  
  // Movement
  glm::vec3 dir = glm::vec3(0.0f);
  
  if (Controls::IsButtonDown(Button::kMoveForward)) {
    dir.z += 1.0f;
  }
  if (Controls::IsButtonDown(Button::kMoveBack)) {
    dir.z -= 1.0f;
  }
  if (Controls::IsButtonDown(Button::kMoveLeft)) {
    dir.x += 1.0f;
  }
  if (Controls::IsButtonDown(Button::kMoveRight)) {
    dir.x -= 1.0f;
  }
  
  if (dir.x != 0.0f || dir.z != 0.0f) {
    dir = glm::rotateY(dir, -rotation.y);
    position += glm::normalize(dir) * kMoveSpeed * delta;
  }
  
  if (Controls::IsButtonDown(Button::kCrouch)) {
    position.y += kMoveSpeed * delta;
  }
  if (Controls::IsButtonDown(Button::kJump)) {
    position.y -= kMoveSpeed * delta;
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
