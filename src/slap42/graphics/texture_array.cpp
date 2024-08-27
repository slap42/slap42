#include "texture_array.hpp"

#include <cstring>
#include <glad/gl.h>
#include <stb_image.h>
#include "gl_check.hpp"

namespace Slap42 {

TextureArray::TextureArray(const char** filenames, size_t file_count) : texture(0) {
  if (file_count < 1) {
    fprintf(stderr, "Cannot create an empty texture array\n");
  }

  constexpr size_t kDesiredChannels = 3;

  int w, h, channel_count;
  stbi_uc* stb_pixels = stbi_load(filenames[0], &w, &h, &channel_count, kDesiredChannels);
  if (!stb_pixels) {
    fprintf(stderr, "Failed to load texture \"%s\"\n", filenames[0]);
    return;
  }
  size_t width = w, height = h;

  size_t texture_size = width * height * kDesiredChannels;
  size_t pixels_size = texture_size * file_count;
  unsigned char* pixels = new unsigned char[pixels_size];
  std::memcpy(pixels, stb_pixels, width * height * kDesiredChannels);

  stbi_image_free(stb_pixels);

  for (size_t i = 1; i < file_count; ++i) {
    stb_pixels = stbi_load(filenames[i], &w, &h, &channel_count, kDesiredChannels);
    if (!stb_pixels) {
      fprintf(stderr, "Failed to load texture \"%s\"\n", filenames[0]);
      return;
    }
    if (w * h * channel_count != texture_size) {
      fprintf(stderr, "Failed to create texture array: \"%s\" is not the same size as a previously loaded texture\n", filenames[0]);
      return;
    }

    size_t offset = texture_size * i;
    std::memcpy(pixels + offset, stb_pixels, texture_size);

    stbi_image_free(stb_pixels);
  }

  GL_CHECK(glGenTextures(1, &texture));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D_ARRAY, texture));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D_ARRAY, texture));
  GL_CHECK(glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, width, height, file_count, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels));
  GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D_ARRAY));

  delete[] pixels;
}

TextureArray::~TextureArray() {
  GL_CHECK(glDeleteTextures(1, &texture));
}

void TextureArray::Bind(int slot) const {
  GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D_ARRAY, texture));
}

}
