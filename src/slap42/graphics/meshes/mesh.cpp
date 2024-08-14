#include "mesh.hpp"

#include <glad/gl.h>

namespace Slap42 {

Mesh::Mesh(float* vertices, int vertices_size_bytes, unsigned short* indices, int indices_size_bytes) : index_count(indices_size_bytes / sizeof(unsigned short)) { 
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ibo);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices_size_bytes, vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size_bytes, indices,  GL_STATIC_DRAW);
}

Mesh::~Mesh() {
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  glDeleteVertexArrays(1, &vao);
}


void Mesh::Render() const {
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_SHORT, 0);
}

}
