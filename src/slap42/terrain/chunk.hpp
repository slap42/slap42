#pragma once

#include "graphics/mesh.hpp"

namespace Slap42 {

class Chunk {
public:
  Chunk(int x, int z);
  ~Chunk();

public:
  void Render() const;

private:
  Mesh* mesh;
};

}
