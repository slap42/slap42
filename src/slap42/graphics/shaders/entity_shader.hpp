#pragma once

#include <glm/glm.hpp>
#include "shader.hpp"

namespace Slap42 {

class EntityShader : public Shader {
public:
  EntityShader();

public:
  void SetViewProjection(const glm::mat4& view_projection);

private:
  unsigned int u_view_projection;
};

}
