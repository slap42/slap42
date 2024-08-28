#pragma once

#include "mesh.hpp"

namespace Slap42 {

class SceneryMesh : public Mesh {
public:
  SceneryMesh(float* vertices, size_t vertices_size_bytes, uint16_t* indices, size_t indices_size_bytes);
};

}
