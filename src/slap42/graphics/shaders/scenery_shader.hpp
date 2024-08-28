#pragma once

#include <glm/glm.hpp>
#include "shader.hpp"

namespace Slap42 {
namespace Shader {
namespace SceneryShader {

void Create();
void Destroy();
void Bind();

void SetViewProjection(const glm::mat4& view_projection);
void SetSunDirection(const glm::vec3& sun_dir);

}
}
}
