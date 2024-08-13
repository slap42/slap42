#include "texture_array.hpp"

#include <cstring>
#include <glad/gl.h>
#include <stb_image.h>

namespace Slap42 {

TextureArray::TextureArray(const char** filenames, int file_count) {
  if (file_count < 1) {
    fprintf(stderr, "Cannot create an empty texture array\n");
  }

  int width, height, channel_count;
  stbi_uc* stb_pixels = stbi_load(filenames[0], &width, &height, &channel_count, 0);
  if (!stb_pixels) {
    fprintf(stderr, "Failed to load texture \"%s\"\n", filenames[0]);
  }

  unsigned char* pixels = new unsigned char[width * height * file_count * 4];
  std::memcpy(pixels, stb_pixels, width * height * 4);

  stbi_image_free(stb_pixels);

  for (int i = 1; i < file_count; ++i) {
    stb_pixels = stbi_load(filenames[i], &width, &height, &channel_count, 0);
    if (!stb_pixels) {
      fprintf(stderr, "Failed to load texture \"%s\"\n", filenames[0]);
    }

    std::memcpy(pixels + width * height * 4 * i, stb_pixels, width * height * 4);

    stbi_image_free(stb_pixels);
  }

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, file_count, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

  delete[] pixels;
}

TextureArray::~TextureArray() {
  glDeleteTextures(1, &texture);
}

void TextureArray::Bind(int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
}

}
