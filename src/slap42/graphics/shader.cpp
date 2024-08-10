#include "shader.hpp"

#include <cstdio>
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace Slap42 {

const char* vert_src = ""
  "#version 330 core\n"
  "layout (location = 0) in vec3 a_pos;"
  "uniform mat4 u_view_projection;"
  "out vec3 v_pos;"
  "void main() {"
  "  gl_Position = u_view_projection * vec4(a_pos, 1.0);"
  "  v_pos = a_pos + 0.5;"
  "}";

const char* frag_src = ""
  "#version 330 core\n"
  "in vec3 v_pos;"
  "out vec4 o_color;"
  "void main() {"
  "  o_color = vec4(v_pos, 1.0);"
  "}";

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

Shader::Shader() {
  GLuint vert_shader = CreateShaderModule(GL_VERTEX_SHADER, &vert_src);
  GLuint frag_shader = CreateShaderModule(GL_FRAGMENT_SHADER, &frag_src);
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

  u_view_projection = glGetUniformLocation(shader_program, "u_view_projection");
}

void Shader::SetViewProjection(const glm::mat4& view_projection) {
  glUniformMatrix4fv(u_view_projection, 1, GL_FALSE, glm::value_ptr(view_projection));
}
 
Shader::~Shader() {
  glDeleteProgram(shader_program);
}

void Shader::Bind() const {
  glUseProgram(shader_program);
}

}
