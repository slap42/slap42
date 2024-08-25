#pragma once

#include <cstddef>

namespace Slap42 {

class Mesh {
public:
  Mesh(float* vertices, size_t vertices_size_bytes, unsigned short* indices, size_t indices_size_bytes);
  virtual ~Mesh();

public:
  void Render() const;

private:
  unsigned int vao, vbo, ibo;
  int index_count;
};

}
