#include "shader.hpp"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <glad/gl.h>

namespace Slap42 {

static GLuint CreateShaderModule(GLenum type, const char* file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    fprintf(stderr, "Failed to open shader file : %s\n", file_path);
  }
  std::stringstream ss;
  ss << file.rdbuf();
  std::string str = ss.str();
  const char* code[1] = { str.c_str() };

  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, code, nullptr);
  glCompileShader(shader);

  int success;
  char info_log[512] { };
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, sizeof(info_log), nullptr, info_log);
    fprintf(stderr, "Shader compile failed: \"%s\"\n Shader failed to compile with error: %s\n", file_path, info_log);
  }

  return shader;
}

Shader::Shader(const char* vert_path, const char* frag_path) {
  GLuint vert_shader = CreateShaderModule(GL_VERTEX_SHADER, vert_path);
  GLuint frag_shader = CreateShaderModule(GL_FRAGMENT_SHADER, frag_path);
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vert_shader);
  glAttachShader(shader_program, frag_shader);
  glLinkProgram(shader_program);
  glUseProgram(shader_program);

  int success;
  char info_log[512] { };
  glGetShaderiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_program, sizeof(info_log), nullptr, info_log);
    fprintf(stderr, "Shader link failed with error: %s\n", info_log);
  }

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
}

Shader::~Shader() {
  glDeleteProgram(shader_program);
}

void Shader::Bind() const {
  glUseProgram(shader_program);
}

}
