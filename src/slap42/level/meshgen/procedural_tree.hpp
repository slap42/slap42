#pragma once

#include <glm/glm.hpp>
#include "raw_mesh.hpp"

namespace Slap42 {
namespace MeshGen {

void Tree(RawMesh& raw_mesh, const glm::vec3& origin, const glm::vec3& direction, size_t segment_count = 13, float segment_radius = 0.5f, bool trunk = true);

}
}
