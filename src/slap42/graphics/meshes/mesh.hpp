#pragma once

namespace Slap42 {

class Mesh {
public:
  Mesh(float* vertices, int vertex_count, unsigned short* indices, int index_count);
  virtual ~Mesh();

public:
  void Render() const;

private:
  unsigned int vao, vbo, ibo;
  int index_count;
};

}
