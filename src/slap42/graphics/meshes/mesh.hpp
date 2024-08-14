#pragma once

namespace Slap42 {

class Mesh {
public:
  Mesh(float* vertices, int vertices_size_bytes, unsigned short* indices, int indices_size_bytes);
  virtual ~Mesh();

public:
  void Render() const;

private:
  unsigned int vao, vbo, ibo;
  int index_count;
};

}
