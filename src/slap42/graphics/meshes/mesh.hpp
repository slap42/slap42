#pragma once

#include <cstddef>
#include <cstdint>

namespace Slap42 {

class Mesh {
public:
  Mesh(float* vertices, size_t vertices_size_bytes, uint16_t* indices, size_t indices_size_bytes);
  virtual ~Mesh();

public:
  void Render() const;

private:
  unsigned int vao, vbo, ibo;
  size_t index_count;
};

}
