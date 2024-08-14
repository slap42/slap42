#pragma once

#include "mesh.hpp"

namespace Slap42 {

class TerrainMesh : public Mesh {
public:
  TerrainMesh(float* vertices, int vertex_count, unsigned short* indices, int index_count);
};

}
