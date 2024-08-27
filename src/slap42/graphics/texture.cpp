#include "texture.hpp"

#include <glad/gl.h>
#include <stb_image.h>
#include "gl_check.hpp"

namespace Slap42 {

Texture::Texture(const char* filename) {
  int width, height, channel_count;
  stbi_uc* pixels = stbi_load(filename, &width, &height, &channel_count, 3);
  if (!pixels) {
    fprintf(stderr, "Failed to load texture \"%s\"\n", filename);
  }

  GL_CHECK(glGenTextures(1, &texture));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels));
  GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));

  stbi_image_free(pixels);
}

Texture::~Texture() {
  GL_CHECK(glDeleteTextures(1, &texture));
}

void Texture::Bind(int slot) const {
  GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture));
}

}
