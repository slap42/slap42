#pragma once

#include <cstddef>

namespace Slap42 {

class TextureArray {
public:
  TextureArray(const char** filenames, size_t file_count);
  ~TextureArray();

public:
  void Bind(int slot = 0) const;

private:
  unsigned int texture;
};

}
