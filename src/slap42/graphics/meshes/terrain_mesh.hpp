#pragma once

#include "mesh.hpp"

namespace Slap42 {

class TerrainMesh : public Mesh {
public:
  TerrainMesh(float* vertices, int vertices_size_bytes, unsigned short* indices, int indices_size_bytes);
};

}
