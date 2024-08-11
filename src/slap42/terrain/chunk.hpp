#pragma once

#include "graphics/mesh.hpp"

namespace Slap42 {

class Chunk {
public:
  Chunk(int chunkx, int chunkz);
  ~Chunk();

public:
  void Render() const;

private:
  Mesh* mesh;
};

}
