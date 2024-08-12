#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

namespace Slap42 {

Camera::Camera(GLFWwindow* window, Shader* shader) : window(window), shader(shader) {
  position = glm::vec3(0.0f, -2.0f, 0.0f);
  CalcView();
  OnResize(1280, 720);
}

void Camera::Update() {
  const float kMoveSpeed = 0.02f;
  const float kRotationSpeed = 0.04f;

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
    if (rotation.x < -1.57f) rotation.x = -1.57f;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN)) {
    rotation.x += kRotationSpeed;
    if (rotation.x > 1.57f) rotation.x = 1.57f;
  }

  // Position
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
