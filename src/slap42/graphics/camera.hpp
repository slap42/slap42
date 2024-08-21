#pragma once

#include <glm/glm.hpp>
#include "shaders/entity_shader.hpp"
#include "shaders/terrain_shader.hpp"

namespace Slap42 {
namespace Camera {

void Create();
void Update();

glm::vec3 GetPosition();

}
}
