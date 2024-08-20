#pragma once

#include <glm/glm.hpp>
#include "shader.hpp"

namespace Slap42 {
namespace Shader {
namespace EntityShader {

void Create();
void Destroy();
void Bind();

void SetTransform(const glm::mat4& transform);
void SetViewProjection(const glm::mat4& view_projection);

}
}
}
