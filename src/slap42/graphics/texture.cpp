#include "texture.hpp"

#include <glad/gl.h>
#include <stb_image.h>

namespace Slap42 {

Texture::Texture(const char* filename) {
  int width, height, channel_count;
  stbi_uc* pixels = stbi_load(filename, &width, &height, &channel_count, 4);
  if (!pixels) {
    fprintf(stderr, "Failed to load texture \"%s\"\n", filename);
  }

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(pixels);
}

Texture::~Texture() {
  glDeleteTextures(1, &texture);
}

void Texture::Bind(int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, texture);
}

}
