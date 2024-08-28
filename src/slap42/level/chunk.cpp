#include "chunk.hpp"

#include <vector>
#include "level/noise.hpp"
#include "meshgen/procedural_terrain.hpp"
#include "meshgen/procedural_tree.hpp"
#include "utils/random.hpp"

namespace Slap42 {

Chunk::Chunk(int chunkx, int chunkz) : x(chunkx), z(chunkz) {
  terrain_mesh = MeshGen::Terrain(chunkx, chunkz);

  MeshGen::RawMesh raw_mesh;
  glm::vec3 origin = {
    chunkx * (float)Chunk::kChunkSize,
    Noise::SampleTerrainHeight(chunkx * (float)Chunk::kChunkSize, chunkz * (float)Chunk::kChunkSize),
    chunkz * (float)Chunk::kChunkSize
  };
  glm::vec3 direction = glm::normalize(glm::vec3(RandMToN(-0.5f, 0.5f), 1.0f, RandMToN(-0.5f, 0.5f)));
  MeshGen::Tree(raw_mesh, origin, direction);
  scenery_mesh = new SceneryMesh(raw_mesh.vertices.data(), raw_mesh.vertices.size() * sizeof(float), raw_mesh.indices.data(), raw_mesh.indices.size() * sizeof(uint16_t));
}

Chunk::~Chunk() {
  delete scenery_mesh;
  delete terrain_mesh;
}

void Chunk::RenderTerrain() const {
  terrain_mesh->Render();
}

void Chunk::RenderScenery() const {
  scenery_mesh->Render();
}

}
