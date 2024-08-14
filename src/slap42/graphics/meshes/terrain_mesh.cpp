#include "terrain_mesh.hpp"

#include <glad/gl.h>

namespace Slap42 {

TerrainMesh::TerrainMesh(float* vertices, int vertex_count, unsigned short* indices, int index_count)
    : Mesh(vertices, vertex_count, indices, index_count) {
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

}
