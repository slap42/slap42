#include "scenery_mesh.hpp"

#include <glad/gl.h>
#include "graphics/gl_check.hpp"

namespace Slap42 {

SceneryMesh::SceneryMesh(float* vertices, size_t vertices_size_bytes, uint16_t* indices, size_t indices_size_bytes)
    : Mesh(vertices, vertices_size_bytes, indices, indices_size_bytes) {
  GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float))));
  GL_CHECK(glEnableVertexAttribArray(0));
  GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
  GL_CHECK(glEnableVertexAttribArray(1));
  GL_CHECK(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))));
  GL_CHECK(glEnableVertexAttribArray(2));
}

}
