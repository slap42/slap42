#include "mesh.hpp"

#include <glad/gl.h>
#include "graphics/gl_check.hpp"

namespace Slap42 {

Mesh::Mesh(float* vertices, size_t vertices_size_bytes, unsigned short* indices, size_t indices_size_bytes) : index_count(indices_size_bytes / sizeof(unsigned short)) { 
  GL_CHECK(glGenVertexArrays(1, &vao));
  GL_CHECK(glGenBuffers(1, &vbo));
  GL_CHECK(glGenBuffers(1, &ibo));
  GL_CHECK(glBindVertexArray(vao));

  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertices_size_bytes, vertices, GL_STATIC_DRAW));

  GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
  GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size_bytes, indices,  GL_STATIC_DRAW));
}

Mesh::~Mesh() {
  GL_CHECK(glDeleteBuffers(1, &vbo));
  GL_CHECK(glDeleteBuffers(1, &ibo));
  GL_CHECK(glDeleteVertexArrays(1, &vao));
}

void Mesh::Render() const {
  GL_CHECK(glBindVertexArray(vao));
  GL_CHECK(glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_SHORT, 0));
}

}
