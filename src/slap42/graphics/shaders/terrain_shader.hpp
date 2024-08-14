#pragma once

#include <glm/glm.hpp>
#include "shader.hpp"

namespace Slap42 {

class TerrainShader : public Shader {
public:
  TerrainShader();

public:
  void SetViewProjection(const glm::mat4& view_projection);

private:
  unsigned int u_view_projection;
};

}
