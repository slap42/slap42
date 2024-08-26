#include "chunk.hpp"

#include <vector>
#include <glm/glm.hpp>
#include "noise.hpp"

namespace Slap42 {

Chunk::Chunk(int chunkx, int chunkz) : x(chunkx), z(chunkz) {
  constexpr float kChunkSizeF = (float)kChunkSize;
  constexpr size_t kVertexSize = 7;
  constexpr size_t kVertexBufferSize = (kChunkSize + 1) * (kChunkSize + 1) * kVertexSize;
  constexpr size_t kIndexBufferSize = kChunkSize * kChunkSize * 6;

  std::vector<float> vertices(kVertexBufferSize);
  std::vector<unsigned short> indices(kIndexBufferSize);

  for (size_t z = 0; z < kChunkSize + 1; ++z) {
    for (size_t x = 0; x < kChunkSize + 1; ++x) {
      size_t i = x * kVertexSize + z * kVertexSize * (kChunkSize + 1);
      float xx = (float)(x + chunkx * kChunkSizeF);
      float zz = (float)(z + chunkz * kChunkSizeF);
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

  for (size_t z = 0; z < kChunkSize; ++z) {
    for (size_t x = 0; x < kChunkSize; ++x) {
      size_t i = x * 6 + z * 6 * kChunkSize;
      size_t j = x + z * (kChunkSize + 1);
      indices[i + 0] = j + 0;
      indices[i + 1] = j + kChunkSize + 1;
      indices[i + 2] = j + 1;
      indices[i + 3] = j + 1;
      indices[i + 4] = j + kChunkSize + 1;
      indices[i + 5] = j + kChunkSize + 2;
    }
  }

  mesh = new TerrainMesh(vertices.data(), vertices.size() * sizeof(float), indices.data(), indices.size() * sizeof(unsigned short));
}

Chunk::~Chunk() {
  delete mesh;
}

void Chunk::Render() const {
  mesh->Render();
}

}
