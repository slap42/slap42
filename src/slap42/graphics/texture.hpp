#pragma once

namespace Slap42 {

class Texture {
public:
  Texture(const char* filename);
  ~Texture();

public:
  void Bind(int slot = 0) const;

private:
  unsigned int texture;
};

}
