#include "chunk.hpp"

#include "noise.hpp"

namespace Slap42 {

Chunk::Chunk(int chunkx, int chunkz) : x(chunkx), z(chunkz) {
  constexpr float kChunkSizeF = (float)kChunkSize;
  constexpr size_t kVertexSize = 4;
  constexpr size_t kVertexBufferSize = (kChunkSize + 1) * (kChunkSize + 1) * kVertexSize;
  constexpr size_t kIndexBufferSize = kChunkSize * kChunkSize * 6;

  float* vertices = new float[kVertexBufferSize];
  unsigned short* indices = new unsigned short[kIndexBufferSize];

  for (size_t z = 0; z < kChunkSize + 1; ++z) {
    for (size_t x = 0; x < kChunkSize + 1; ++x) {
      size_t i = x * kVertexSize + z * kVertexSize * (kChunkSize + 1);
      float xx = (float)(x + chunkx * kChunkSizeF);
      float zz = (float)(z + chunkz * kChunkSizeF);
      vertices[i + 0] = xx;
      vertices[i + 1] = Noise::SampleHeight(xx, zz);
      vertices[i + 2] = zz;
      vertices[i + 3] = Noise::SampleTexture(xx, zz);
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

  mesh = new TerrainMesh(vertices, kVertexBufferSize * sizeof(float), indices, kIndexBufferSize * sizeof(unsigned short));

  delete[] vertices;
  delete[] indices;
}

Chunk::~Chunk() {
  delete mesh;
}

void Chunk::Render() const {
  mesh->Render();
}

}
