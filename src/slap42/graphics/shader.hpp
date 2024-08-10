#pragma once

#include <glm/glm.hpp>

namespace Slap42 {

class Shader {
public:
  Shader();
  ~Shader();

public:
  void Bind() const;

public:
  void SetViewProjection(const glm::mat4& view_projection);

private:
  unsigned int shader_program;
  unsigned int u_view_projection;
};

}
