#include "camera.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <imgui.h>
#include "gl_check.hpp"
#include "level/noise.hpp"
#include "networking/client/client.hpp"
#include "shaders/entity_shader.hpp"
#include "shaders/scenery_shader.hpp"
#include "shaders/terrain_shader.hpp"
#include "window/controls.hpp"
#include "window/window.hpp"

namespace Slap42 {
namespace Camera {

static glm::vec3 position;
static glm::vec2 rotation;
static glm::mat4 view;
static glm::mat4 projection;
static glm::mat4 view_projection;

static float fov = 1.5f;

static bool flag_vp_dirty = false;

bool IsVpDirty() {
  return flag_vp_dirty;
}

const glm::mat4& GetViewProjection() {
  return view_projection;
}

static void CalcViewProjection() {
  view_projection = projection * view;
  flag_vp_dirty = true;
}

static void CalcView() {
  constexpr glm::vec3 kXAxis(1.0f, 0.0f, 0.0f);
  constexpr glm::vec3 kYAxis(0.0f, 1.0f, 0.0f);
  
  view = glm::rotate(glm::mat4(1.0f), rotation.x, kXAxis);
  view = glm::rotate(view, rotation.y, kYAxis);
  view = glm::translate(view, position);
  CalcViewProjection();
}

static void CalcProjection() {
  projection = glm::perspectiveFov(fov, (float)Window::GetFramebufferWidth(), (float)Window::GetFramebufferHeight(), 0.1f, 10000.0f);
  CalcViewProjection();
}

static void OnResize(int width, int height) {
  if (width <= 0 || height <= 0) return;
  CalcProjection();
}

float GetFov() {
  return fov;
}

void SetFov(float f) {
  fov = f;
  CalcProjection();
}

void Create() {
  CalcView();
  CalcProjection();
  
  auto on_resize = [](GLFWwindow* window, int w, int h) {
    if (w <= 0 || h <= 0) return;
    OnResize(w, h);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    GL_CHECK(glViewport(0, 0, width, height));
  };
  glfwSetWindowSizeCallback(Window::GetGlfwWindow(), on_resize);
  on_resize(Window::GetGlfwWindow(), Window::GetFramebufferWidth(), Window::GetFramebufferHeight());
}

void Update(float delta) {
  flag_vp_dirty = false;
  
  static GLFWwindow* window = Window::GetGlfwWindow();
  
  constexpr float kRotationSpeed = 0.04f;
  constexpr float kMouseSensitivity = 0.005f;
  
  glm::vec3 old_position = position;
  glm::vec2 old_rotation = rotation;
  
  // Speed in meters per second
  constexpr float kFlyingSpeed = 20.0f;
  constexpr float kWalkingSpeed = 1.42f;
  static float move_speed = kFlyingSpeed * 0.001f;
  static bool flying = true;
  if (Controls::IsButtonPressed(Button::kToggleFlying)) {
    flying = !flying;
    move_speed = flying ? kFlyingSpeed * 0.001f : kWalkingSpeed * 0.001f;
    position.x += 0.0001f;
  }
  
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
    position += glm::normalize(dir) * move_speed * delta;
  }
  
  if (Controls::IsButtonDown(Button::kCrouch)) {
    position.y += move_speed * delta;
  }
  if (Controls::IsButtonDown(Button::kJump)) {
    position.y -= move_speed * delta;
  }
  
  if (position != old_position || rotation != old_rotation) {
    if (!flying) {
      position.y = -Noise::SampleTerrainHeight(-position.x, -position.z) - 1.7f;
    }
    CalcView();
    Client::SendPositionUpdate(position, rotation);
  }
}

void ResetPosition() {
  position = glm::vec3{ 0.0f };
  rotation = glm::vec2{ 0.0f };
  CalcView();
}

void SetPosition(const glm::vec3& pos) {
  position = pos;
  CalcView();
  Client::SendPositionUpdate(position, rotation);
}

glm::vec3 GetPosition() {
  return position;
}

}
}
