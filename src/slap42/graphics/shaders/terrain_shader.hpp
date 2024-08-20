#pragma once

#include <glm/glm.hpp>
#include "shader.hpp"

namespace Slap42 {
namespace Shader {
namespace TerrainShader {

void Create();
void Destroy();
void Bind();

void SetViewProjection(const glm::mat4& view_projection);

}
}
}
