#pragma once

#include "mesh.hpp"

namespace Slap42 {

class TerrainMesh : public Mesh {
public:
  TerrainMesh(float* vertices, size_t vertices_size_bytes, unsigned short* indices, size_t indices_size_bytes);

public:
  static constexpr size_t kVertexSize = 7;
};

}
