#include "chunk.hpp"

#include <vector>
#include "meshgen/procedural_terrain.hpp"
#include "meshgen/procedural_tree.hpp"

namespace Slap42 {

Chunk::Chunk(int chunkx, int chunkz) : x(chunkx), z(chunkz) {

  std::vector<float> vertices;
  std::vector<uint16_t> indices;
  size_t current_index = 0;

  MeshGen::Terrain(vertices, indices, current_index, chunkx * kChunkSize, chunkz * kChunkSize);

  for (int x = 0; x < kChunkSize; ++x) {
    for (int z = 0; z < kChunkSize; ++z) {
      MeshGen::Tree(vertices, indices, current_index, chunkx * kChunkSize + x, chunkz * kChunkSize + z);
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
