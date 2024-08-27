#include "procedural_terrain.hpp"

#include <glm/glm.hpp>
#include "level/chunk.hpp"
#include "level/noise.hpp"

namespace Slap42 {
namespace MeshGen {

void Terrain(std::vector<float>& vertices, std::vector<uint16_t>& indices, size_t current_index, int xoffs, int zoffs) {
  constexpr float kChunkSizeF = (float)Chunk::kChunkSize;
  constexpr size_t kVertexBufferSize = (Chunk::kChunkSize + 1) * (Chunk::kChunkSize + 1) * TerrainMesh::kVertexSize;
  constexpr size_t kIndexBufferSize = Chunk::kChunkSize * Chunk::kChunkSize * 6;

  current_index += Chunk::kChunkSize * Chunk::kChunkSize * 2;

  vertices.resize(kVertexBufferSize);
  indices.resize(kIndexBufferSize);

  for (size_t z = 0; z < Chunk::kChunkSize + 1; ++z) {
    for (size_t x = 0; x < Chunk::kChunkSize + 1; ++x) {

      size_t i = x * TerrainMesh::kVertexSize + z * TerrainMesh::kVertexSize * (Chunk::kChunkSize + 1);
      float xx = (float)x + (float)xoffs;
      float zz = (float)z + (float)zoffs;

      // Position
      vertices[i + 0] = xx;
      vertices[i + 1] = Noise::SampleTerrainHeight(xx, zz);
      vertices[i + 2] = zz;
      
      // Texture
      vertices[i + 3] = Noise::SampleTerrainTexture(xx, zz);
      
      // Normal
      glm::vec3 p = glm::vec3(xx, Noise::SampleTerrainHeight(xx, zz), zz);
      
      glm::vec3 l = glm::vec3(xx - 0.1f, Noise::SampleTerrainHeight(xx - 0.1f, zz       ), zz       );
      glm::vec3 r = glm::vec3(xx + 0.1f, Noise::SampleTerrainHeight(xx + 0.1f, zz       ), zz       );
      glm::vec3 u = glm::vec3(xx,        Noise::SampleTerrainHeight(xx,        zz - 0.1f), zz - 0.1f);
      glm::vec3 d = glm::vec3(xx,        Noise::SampleTerrainHeight(xx,        zz + 0.1f), zz + 0.1f);
      
      glm::vec3 n0 = glm::normalize(glm::cross(l - p, u - p));
      glm::vec3 n1 = glm::normalize(glm::cross(u - p, r - p));
      glm::vec3 n2 = glm::normalize(glm::cross(r - p, d - p));
      glm::vec3 n3 = glm::normalize(glm::cross(d - p, l - p));
      
      glm::vec3 n = (n0 + n1 + n2 + n3) / 4.0f;
      
      vertices[i + 4] = n.x;
      vertices[i + 5] = n.y;
      vertices[i + 6] = n.z;
    }
  }

  for (size_t z = 0; z < Chunk::kChunkSize; ++z) {
    for (size_t x = 0; x < Chunk::kChunkSize; ++x) {
      size_t i = x * 6 + z * 6 * Chunk::kChunkSize;
      size_t j = x + z * (Chunk::kChunkSize + 1);
      indices[i + 0] = j + 0;
      indices[i + 1] = j + Chunk::kChunkSize + 1;
      indices[i + 2] = j + 1;
      indices[i + 3] = j + 1;
      indices[i + 4] = j + Chunk::kChunkSize + 1;
      indices[i + 5] = j + Chunk::kChunkSize + 2;
    }
  }
}

}
}
