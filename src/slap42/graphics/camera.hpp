#pragma once

#include <glm/glm.hpp>
#include "shader.hpp"

namespace Slap42 {

class Camera {
public:
  Camera(Shader* shader);

public:
  void OnResize(int width, int height);

private:
  glm::mat4 projection = glm::mat4(1.0f);

  Shader* shader;
};

}
