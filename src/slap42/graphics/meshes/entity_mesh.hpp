#pragma once

#include "mesh.hpp"

namespace Slap42 {

class EntityMesh : public Mesh {
public:
  EntityMesh(float* vertices, int vertex_count, unsigned short* indices, int index_count);
};

}
