#include "shader.hpp"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <glad/gl.h>
#include "graphics/gl_check.hpp"

namespace Slap42 {
namespace Shader {

static GLuint CreateShaderModule(GLenum type, const char* file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    fprintf(stderr, "Failed to open shader file : %s\n", file_path);
  }
  std::stringstream ss;
  ss << file.rdbuf();
  std::string str = ss.str();
  const char* code[1] = { str.c_str() };
  
  GLuint shader;
  GL_CHECK(shader = glCreateShader(type));
  GL_CHECK(glShaderSource(shader, 1, code, nullptr));
  GL_CHECK(glCompileShader(shader));
  
  int success;
  char info_log[512] { };
  GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
  if (!success) {
    GL_CHECK(glGetShaderInfoLog(shader, sizeof(info_log), nullptr, info_log));
    fprintf(stderr, "Shader compile failed: \"%s\"\n Shader failed to compile with error: %s\n", file_path, info_log);
  }
  
  return shader;
}

unsigned int Compile(const char* vert_path, const char* frag_path) {
  GLuint vert_shader = CreateShaderModule(GL_VERTEX_SHADER, vert_path);
  GLuint frag_shader = CreateShaderModule(GL_FRAGMENT_SHADER, frag_path);
  unsigned int shader_program;
  GL_CHECK(shader_program = glCreateProgram());
  GL_CHECK(glAttachShader(shader_program, vert_shader));
  GL_CHECK(glAttachShader(shader_program, frag_shader));
  GL_CHECK(glLinkProgram(shader_program));
  
  int success;
  char info_log[512] { };
  GL_CHECK(glGetShaderiv(shader_program, GL_LINK_STATUS, &success));
  if (!success) {
    GL_CHECK(glGetShaderInfoLog(shader_program, sizeof(info_log), nullptr, info_log));
    fprintf(stderr, "Shader link failed with error: %s\n", info_log);
  }
  
  GL_CHECK(glDeleteShader(vert_shader));
  GL_CHECK(glDeleteShader(frag_shader));
  
  return shader_program;
}

}
}
