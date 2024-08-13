#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <imgui.h>

namespace Slap42 {

Camera::Camera(GLFWwindow* window, Shader* shader) : window(window), shader(shader) {
  position = glm::vec3(0.0f, -2.0f, 0.0f);
  CalcView();
  OnResize(1280, 720);
}

void Camera::Update() {
  const float kMoveSpeed = 0.05f;
  const float kRotationSpeed = 0.04f;
  const float kMouseSensitivity = 0.005f;

  glm::vec3 old_position = position;
  glm::vec3 old_rotation = rotation;

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
  double mousex = 0.0, mousey = 0.0;
  static double mousex_old, mousey_old;

  // TODO: move this to an event consumer
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetCursorPos(window, &mousex, &mousey);
    mousex_old = mousex;
    mousey_old = mousey;
  }

  if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
    glfwGetCursorPos(window, &mousex, &mousey);
    double delta_x = mousex - mousex_old;
    double delta_y = mousey - mousey_old;
    mousex_old = mousex;
    mousey_old = mousey;

    rotation.y += (float)delta_x * kMouseSensitivity;
    rotation.x += (float)delta_y * kMouseSensitivity;
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
  }
}

void Camera::OnResize(int width, int height) {
  if (width <= 0 || height <= 0) return;
  projection = glm::perspectiveFov(1.22f, (float)width, float(height), 0.1f, 1000.0f);
  CalcViewProjection();
}

void Camera::CalcView() {
  const glm::vec3 kXAxis(1.0f, 0.0f, 0.0f);
  const glm::vec3 kYAxis(0.0f, 1.0f, 0.0f);

  view = glm::rotate(glm::mat4(1.0f), rotation.x, kXAxis);
  view = glm::rotate(view, rotation.y, kYAxis);
  view = glm::translate(view, position);
  CalcViewProjection();
}

void Camera::CalcViewProjection() {
  glm::mat4 view_projection = projection * view;
  shader->Bind();
  shader->SetViewProjection(view_projection);
}

}
