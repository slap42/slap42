#include "procedural_tree.hpp"

#include <cmath>

namespace Slap42 {
namespace MeshGen {

static float RandZeroToOne() {
  return rand() / (RAND_MAX + 1.0f);
}

void Tree(RawMesh& m, const glm::vec3& origin, const glm::vec3& direction) {
  constexpr float k2Pi = 6.28318530718f;

  constexpr size_t kSegmentPolyCount = 5;
  constexpr float  kSegmentLength = 5.0f;
  constexpr float  kSegmentThickness = 2.5f;
  constexpr size_t kSegmentCount = 5;

  // Add an offset to the orientation of the branch to make it seem more random
  const float kRotationOffset = origin.x * 0.17f + origin.y * 0.79f + + origin.z * 0.13f;

  glm::vec3 pos = origin;

  for (size_t j = 0; j < kSegmentCount; ++j) {
    for (size_t i = 0; i < kSegmentPolyCount; ++i) {
      float frac = ((float)i / (float)kSegmentPolyCount) * k2Pi;

      // TODO: Change this to be 3D rotation around the direction vector
      float x = pos.x + std::sin(frac + kRotationOffset) * kSegmentThickness;
      float y = pos.y;
      float z = pos.z + std::cos(frac + kRotationOffset) * kSegmentThickness;

      m.vertices.push_back(x);
      m.vertices.push_back(y);
      m.vertices.push_back(z);

      m.vertices.push_back(RandZeroToOne());
      m.vertices.push_back(RandZeroToOne());
    }

    pos += direction * kSegmentLength;
  }

  for (size_t j = 0; j < kSegmentCount - 1; ++j) {
    for (size_t i = 0; i < kSegmentPolyCount; ++i) {
      size_t v0 = m.current_index + j * kSegmentPolyCount + i;
      size_t v1 = m.current_index + j * kSegmentPolyCount + (i + 1) % kSegmentPolyCount;
      size_t v2 = m.current_index + j * kSegmentPolyCount + i + kSegmentPolyCount;
      size_t v3 = m.current_index + j * kSegmentPolyCount + kSegmentPolyCount + (i + 1) % kSegmentPolyCount;

      m.indices.push_back(v0);
      m.indices.push_back(v3);
      m.indices.push_back(v2);
      m.indices.push_back(v1);
      m.indices.push_back(v3);
      m.indices.push_back(v0);
    }
  }
  m.current_index += kSegmentPolyCount * kSegmentCount;
}

}
}
