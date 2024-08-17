#include "terrain_shader.hpp"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace Slap42 {

static const char* vert_src = ""
  "#version 330 core\n"
  "layout (location = 0) in vec3 a_pos;"
  "layout (location = 1) in float a_tex_blend;"
  "out vec2 v_tex;"
  "out float v_tex_blend;"
  "uniform mat4 u_view_projection;"
  "void main() {"
  "  v_tex = vec2(a_pos.x / 32.0, a_pos.z / 32.0);"
  "  v_tex_blend = a_tex_blend;"
  "  gl_Position = u_view_projection * vec4(a_pos, 1.0);"
  "}";

static const char* frag_src = ""
  "#version 330 core\n"
  "in vec2 v_tex;"
  "in float v_tex_blend;"
  "out vec4 o_color;"
  "uniform sampler2D u_texture_grass;"
  "uniform sampler2D u_texture_dirt;"
  "void main() {"
  "  o_color = mix(texture(u_texture_grass, v_tex), texture(u_texture_dirt, v_tex), v_tex_blend);"
  "}";

TerrainShader::TerrainShader() : Shader(&vert_src, &frag_src) {
  u_view_projection = glGetUniformLocation(shader_program, "u_view_projection");
  glUniform1i(glGetUniformLocation(shader_program, "u_texture_grass"), 0);
  glUniform1i(glGetUniformLocation(shader_program, "u_texture_dirt"), 1);
}

void TerrainShader::SetViewProjection(const glm::mat4& view_projection) {
  Bind();
  glUniformMatrix4fv(u_view_projection, 1, GL_FALSE, glm::value_ptr(view_projection));
}

}
