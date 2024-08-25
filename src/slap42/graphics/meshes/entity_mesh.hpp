#pragma once

#include "mesh.hpp"

namespace Slap42 {

class EntityMesh : public Mesh {
public:
  EntityMesh(float* vertices, size_t vertices_size_bytes, unsigned short* indices, size_t indices_size_bytes);
};

}
