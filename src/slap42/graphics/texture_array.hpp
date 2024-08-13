#pragma once

namespace Slap42 {

class TextureArray {
public:
  TextureArray(const char** filenames, int file_count);
  ~TextureArray();

public:
  void Bind(int slot = 0) const;

private:
  unsigned int texture;
};

}
