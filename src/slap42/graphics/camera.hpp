#pragma once

#include <glm/glm.hpp>

namespace Slap42 {
namespace Camera {

void Create();
void Update(float delta);
void ResetPosition();

void SetPosition(const glm::vec3& pos);
glm::vec3 GetPosition();

float GetFov();
void SetFov(float fov);

}
}
