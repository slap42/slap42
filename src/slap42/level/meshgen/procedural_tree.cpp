#include "procedural_tree.hpp"

#include <cmath>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "utils/random.hpp"
#include "level/noise.hpp"

namespace Slap42 {
namespace MeshGen {

void Tree(RawMesh& m, const glm::vec3& origin, const glm::vec3& direction, size_t segment_count, float og_segment_radius, bool trunk) {
  if (segment_count == 0) return;

  constexpr float k2Pi = 6.28318530718f;
  constexpr size_t kSegmentPolyCount = 5;
  constexpr float  kSegmentLength = 1.0f;
  constexpr float kBranchTwistiness = 0.2f;

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> directions;
  std::vector<float> segment_radiuses;
  glm::vec3 pos = { origin.x, origin.y, origin.z };
  glm::vec3 dir = direction;
  glm::vec3 otherdir = { std::abs(dir.y), std::abs(dir.x), std::abs(dir.z) };
  
  for (size_t j = 0; j < segment_count; ++j) {
    float v = (float)j / (float)segment_count;

    float segment_radius = og_segment_radius * (1.0f - v);
    segment_radiuses.push_back(segment_radius);
    
    for (size_t i = 0; i < kSegmentPolyCount; ++i) {
      float angle = ((float)i / (float)kSegmentPolyCount) * k2Pi;
      glm::vec3 n = glm::rotate(glm::normalize(glm::cross(dir, otherdir)), angle, dir);

      float x = pos.x + n.x * segment_radius;
      float y = pos.y + n.y * segment_radius;
      float z = pos.z + n.z * segment_radius;

      m.vertices.push_back(x);
      m.vertices.push_back(y);
      m.vertices.push_back(z);
      
      float u = ((float)i / (float)(kSegmentPolyCount - 1)) * segment_radius + (1.0f - segment_radius) / 2.0f;
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
    directions.push_back(dir);

    positions.push_back(pos);
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

  for (size_t j = 0; j < segment_count - 1; ++j) {
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
    size_t v0 = m.current_index + (segment_count - 1) * kSegmentPolyCount + i;
    size_t v1 = m.current_index + (segment_count - 1) * kSegmentPolyCount + (i + 1) % kSegmentPolyCount;
    size_t v2 = m.current_index + segment_count * kSegmentPolyCount;
    
    m.indices.push_back(v0);
    m.indices.push_back(v1);
    m.indices.push_back(v2);
  }
  
  m.current_index += kSegmentPolyCount * segment_count + 1;

  for (size_t i = 1; i < segment_count; ++i) {
    if (RandZeroToOne() > 0.6f) {
      if (trunk && Noise::SampleTerrainHeight(positions[i].x, positions[i].z) + 2.0f > positions[i].y) {
        continue;
      }

      float angle = RandMToN(0.0f, k2Pi);
      glm::vec3 n = glm::rotate(glm::normalize(glm::cross(directions[i], otherdir)), angle, directions[i]);
      n.y = std::abs(n.y);

      Tree(m, positions[i], n, segment_count - i - 1, segment_radiuses[i] * 0.75f, false);
    }
  }
}

}
}
