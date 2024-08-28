#include "procedural_tree.hpp"

#include <cmath>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "utils/random.hpp"

namespace Slap42 {
namespace MeshGen {

void Tree(RawMesh& m, const glm::vec3& origin, const glm::vec3& direction) {
  constexpr float k2Pi = 6.28318530718f;

  constexpr size_t kSegmentPolyCount = 5;
  constexpr float  kSegmentLength = 1.0f;
  constexpr size_t kSegmentCount = 5;
  constexpr float kBranchTwistiness = 0.2f;
  
  float segment_radius = 0.25f;

  // Add an offset to the orientation of the branch to make it seem more random
  const float kRotationOffset = origin.x * 0.17f + origin.y * 0.79f + + origin.z * 0.13f;

  glm::vec3 pos = { origin.x, origin.y - 0.5f, origin.z };
  glm::vec3 dir = direction;

  for (size_t j = 0; j < kSegmentCount; ++j) {
    float v = (float)j / (float)kSegmentCount;
    
    for (size_t i = 0; i < kSegmentPolyCount; ++i) {
      float angle = ((float)i / (float)kSegmentPolyCount) * k2Pi;
      glm::vec3 n = glm::rotate(glm::vec3(0.0f, 0.0f, 1.0f), angle, dir);
      
      float x = pos.x + n.x * segment_radius;
      float y = pos.y + n.y * segment_radius;
      float z = pos.z + n.z * segment_radius;

      m.vertices.push_back(x);
      m.vertices.push_back(y);
      m.vertices.push_back(z);
      
      float u = (float)i / (float)(kSegmentPolyCount - 1);
      m.vertices.push_back(u);
      m.vertices.push_back(v);
      
      m.vertices.push_back(n.x);
      m.vertices.push_back(n.y);
      m.vertices.push_back(n.z);
    }

    dir += glm::vec3(
      RandMToN(-kBranchTwistiness, kBranchTwistiness),
      std::abs(RandMToN(-kBranchTwistiness, kBranchTwistiness)),
      RandMToN(-kBranchTwistiness, kBranchTwistiness)
    );
    dir = glm::normalize(dir);
    segment_radius *= 0.8f;
    pos += dir * kSegmentLength;
  }
  
  m.vertices.push_back(pos.x);
  m.vertices.push_back(pos.y);
  m.vertices.push_back(pos.z);
    
  m.vertices.push_back(0.5f);
  m.vertices.push_back(1.0f);
  
  m.vertices.push_back(dir.x);
  m.vertices.push_back(dir.y);
  m.vertices.push_back(dir.z);

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
  
  for (size_t i = 0; i < kSegmentPolyCount; ++i) {
    size_t v0 = m.current_index + (kSegmentCount - 1) * kSegmentPolyCount + i;
    size_t v1 = m.current_index + (kSegmentCount - 1) * kSegmentPolyCount + (i + 1) % kSegmentPolyCount;
    size_t v2 = m.current_index + kSegmentCount * kSegmentPolyCount;
    
    m.indices.push_back(v0);
    m.indices.push_back(v1);
    m.indices.push_back(v2);
  }
  
  m.current_index += kSegmentPolyCount * kSegmentCount + 1;
}

}
}
