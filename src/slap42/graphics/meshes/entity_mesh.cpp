#include "entity_mesh.hpp"

#include <glad/gl.h>

namespace Slap42 {

EntityMesh::EntityMesh(float* vertices, int vertex_count, unsigned short* indices, int index_count)
    : Mesh(vertices, vertex_count, indices, index_count) {
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

}
