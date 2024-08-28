#pragma once

#include <cstddef>
#include "graphics/meshes/scenery_mesh.hpp"
#include "graphics/meshes/terrain_mesh.hpp"

namespace Slap42 {

class Chunk {
public:
  Chunk(int chunkx, int chunkz);
  ~Chunk();

public:
  void RenderTerrain() const;
  void RenderScenery() const;

public:
  constexpr static size_t kChunkSize = 128;

public:
  inline int GetX() const { return x; }
  inline int GetZ() const { return z; }

private:
  SceneryMesh* scenery_mesh;
  TerrainMesh* terrain_mesh;
  int x, z;
};

}
