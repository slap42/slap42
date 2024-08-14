#pragma once

#include <cstddef>
#include "graphics/meshes/terrain_mesh.hpp"

namespace Slap42 {

class Chunk {
public:
  Chunk(int chunkx, int chunkz);
  ~Chunk();

public:
  void Render() const;

public:
  constexpr static size_t kChunkSize = 32;

public:
  inline int GetX() const { return x; }
  inline int GetZ() const { return z; }

private:
  TerrainMesh* mesh;
  int x, z;
};

}
