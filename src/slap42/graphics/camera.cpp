#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Slap42 {

Camera::Camera(Shader* shader) : shader(shader) {
  OnResize(1280, 720);
}

void Camera::OnResize(int width, int height) {
  projection = glm::perspectiveFov(3.14f / 2.0f, (float)width, float(height), 0.1f, 1000.0f);

  shader->Bind();
  shader->SetViewProjection(projection);
}

}
