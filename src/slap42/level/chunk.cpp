#include "chunk.hpp"

#include <vector>
#include "level/noise.hpp"
#include "meshgen/procedural_terrain.hpp"
#include "meshgen/procedural_tree.hpp"
#include "utils/random.hpp"

namespace Slap42 {

Chunk::Chunk(int chunkx, int chunkz) : x(chunkx), z(chunkz) {
  LoadMeshes();
}

Chunk::~Chunk() {
  if (meshes_loaded) {
    delete scenery_mesh;
    delete terrain_mesh;
  }
}

void Chunk::LoadMeshes() {
  terrain_mesh = MeshGen::Terrain(x, z);

  MeshGen::RawMesh raw_mesh;

  // For each chunk, check whether it contains a tree
  for (size_t xx = 0; xx < kChunkSize; ++xx) {
    for (size_t zz = 0; zz < kChunkSize; ++zz) {
      if (Noise::SampleTrees(x * (float)Chunk::kChunkSize + xx, z * (float)Chunk::kChunkSize + zz)) {
            
        glm::vec3 origin = {
          x * (float)Chunk::kChunkSize + xx,
          Noise::SampleTerrainHeight(x * (float)Chunk::kChunkSize + xx, z * (float)Chunk::kChunkSize + zz) - 0.5f,
          z * (float)Chunk::kChunkSize + zz
        };
        glm::vec3 direction = glm::normalize(glm::vec3(RandMToN(-0.5f, 0.5f), 1.0f, RandMToN(-0.5f, 0.5f)));
        MeshGen::Tree(raw_mesh, origin, direction);

      }
    }
  }

  scenery_mesh = new SceneryMesh(raw_mesh.vertices.data(), raw_mesh.vertices.size() * sizeof(float), raw_mesh.indices.data(), raw_mesh.indices.size() * sizeof(uint16_t));
  
  meshes_loaded = true;
}

void Chunk::RenderTerrain() const {
  terrain_mesh->Render();
}

void Chunk::RenderScenery() const {
  scenery_mesh->Render();
}

}
