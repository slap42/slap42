#include "terrain_shader.hpp"

#include <cstdio>
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace Slap42 {

static const char* vert_src = ""
  "#version 330 core\n"
  "layout (location = 0) in vec3 a_pos;"
  "layout (location = 1) in vec3 a_tex;"
  "out vec3 v_tex;"
  "uniform mat4 u_view_projection;"
  "void main() {"
  "  v_tex = a_tex;"
  "  gl_Position = u_view_projection * vec4(a_pos, 1.0);"
  "}";

static const char* frag_src = ""
  "#version 330 core\n"
  "in vec3 v_tex;"
  "out vec4 o_color;"
  "uniform sampler2DArray u_texture_array;"
  "void main() {"
  "  o_color = texture(u_texture_array, v_tex);"
  "}";

TerrainShader::TerrainShader() : Shader(&vert_src, &frag_src) {
  u_view_projection = glGetUniformLocation(shader_program, "u_view_projection");
  glUniform1i(glGetUniformLocation(shader_program, "u_texture_array"), 0);
}

void TerrainShader::SetViewProjection(const glm::mat4& view_projection) {
  glUniformMatrix4fv(u_view_projection, 1, GL_FALSE, glm::value_ptr(view_projection));
}

}
