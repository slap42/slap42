#include "terrain_mesh.hpp"

#include <glad/gl.h>

namespace Slap42 {

TerrainMesh::TerrainMesh(float* vertices, int vertices_size_bytes, unsigned short* indices, int indices_size_bytes)
    : Mesh(vertices, vertices_size_bytes, indices, indices_size_bytes) {
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

}
