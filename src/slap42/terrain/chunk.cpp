#include "chunk.hpp"

#include "OpenSimplexNoise.hh"

namespace Slap42 {

Chunk::Chunk(int chunkx, int chunkz) {
  constexpr size_t kChunkSize = 32;
  constexpr float kChunkSizeF = (float)kChunkSize;
  constexpr size_t kVertexSize = 5;
  constexpr size_t kVertexBufferSize = kChunkSize * kChunkSize * kVertexSize * 4;
  constexpr size_t kIndexBufferSize = kChunkSize * kChunkSize * 6;

  OSN::Noise<2> noise;
  constexpr float kHeight = 5.0f;
  constexpr float kSample = 0.1f;

  float* vertices = new float[kVertexBufferSize];
  unsigned short* indices = new unsigned short[kIndexBufferSize];

  for (size_t z = 0; z < kChunkSize; ++z) {
    for (size_t x = 0; x < kChunkSize; ++x) {
      size_t i = x * kVertexSize * 4 + z * kChunkSize * kVertexSize * 4;
      float xx = (float)(x + chunkx * kChunkSizeF);
      float zz = (float)(z + chunkz * kChunkSizeF);
      vertices[i +  0] = 0.0f + xx; vertices[i +  1] = noise.eval((0.0f + xx) * kSample, (1.0f + zz) * kSample) * kHeight; vertices[i +  2] = 1.0f + zz; vertices[i +  3] = 0.0f; vertices[i +  4] = 1.0f;
      vertices[i +  5] = 1.0f + xx; vertices[i +  6] = noise.eval((1.0f + xx) * kSample, (1.0f + zz) * kSample) * kHeight; vertices[i +  7] = 1.0f + zz; vertices[i +  8] = 1.0f; vertices[i +  9] = 1.0f;
      vertices[i + 10] = 0.0f + xx; vertices[i + 11] = noise.eval((0.0f + xx) * kSample, (0.0f + zz) * kSample) * kHeight; vertices[i + 12] = 0.0f + zz; vertices[i + 13] = 0.0f; vertices[i + 14] = 0.0f;
      vertices[i + 15] = 1.0f + xx; vertices[i + 16] = noise.eval((1.0f + xx) * kSample, (0.0f + zz) * kSample) * kHeight; vertices[i + 17] = 0.0f + zz; vertices[i + 18] = 1.0f; vertices[i + 19] = 0.0f;

      size_t j = x * 6 + z * kChunkSize * 6;
      size_t k = x * 4 + z * kChunkSize * 4;
      indices[j + 0] = 0 + k; indices[j + 1] = 1 + k; indices[j + 2] = 2 + k;
      indices[j + 3] = 2 + k; indices[j + 4] = 1 + k; indices[j + 5] = 3 + k;
    }
  }

  mesh = new Mesh(vertices, kVertexBufferSize * sizeof(float), indices, kIndexBufferSize * sizeof(unsigned short));

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
