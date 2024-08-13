#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader.hpp"

namespace Slap42 {

class Camera {
public:
  Camera(GLFWwindow* window, Shader* shader);

public:
  void Update();

public:
  void OnResize(int width, int height);

private:
  void CalcView();
  void CalcViewProjection();

public:
  inline glm::vec3 GetPosition() const { return position; }

private:
  // Owned stuff
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 rotation = glm::vec3(0.0f);

  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  // Non-owned pointers
  GLFWwindow* window;
  Shader* shader;
};

}
