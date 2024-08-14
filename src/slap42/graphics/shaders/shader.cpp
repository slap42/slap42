#include "shader.hpp"

#include <cstdio>
#include <glad/gl.h>

namespace Slap42 {

static GLuint CreateShaderModule(GLenum type, const char** code) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, code, nullptr);
  glCompileShader(shader);

  int success;
  char info_log[512] { };
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, sizeof(info_log), nullptr, info_log);
    fprintf(stderr, "Shader compile failed with error: %s\n", info_log);
  }

  return shader;
}

Shader::Shader(const char** vert_src, const char** frag_src) {
  GLuint vert_shader = CreateShaderModule(GL_VERTEX_SHADER, vert_src);
  GLuint frag_shader = CreateShaderModule(GL_FRAGMENT_SHADER, frag_src);
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vert_shader);
  glAttachShader(shader_program, frag_shader);
  glLinkProgram(shader_program);

  int success;
  char info_log[512] { };
  glGetShaderiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_program, sizeof(info_log), nullptr, info_log);
    fprintf(stderr, "Shader link failed with error: %s\n", info_log);
  }

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
  glUseProgram(shader_program);
}

Shader::~Shader() {
  glDeleteProgram(shader_program);
}

void Shader::Bind() const {
  glUseProgram(shader_program);
}

}
