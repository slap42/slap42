#pragma once

#include "mesh.hpp"

namespace Slap42 {

class EntityMesh : public Mesh {
public:
  EntityMesh(float* vertices, int vertices_size_bytes, unsigned short* indices, int indices_size_bytes);
};

}
