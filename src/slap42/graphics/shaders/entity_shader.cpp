#include "entity_shader.hpp"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace Slap42 {

static const char* vert_src = ""
  "#version 330 core\n"
  "layout (location = 0) in vec3 a_pos;"
  "layout (location = 1) in vec2 a_tex;"
  "out vec2 v_tex;"
  "uniform mat4 u_transform;"
  "uniform mat4 u_view_projection;"
  "void main() {"
  "  v_tex = a_tex;"
  "  gl_Position = u_view_projection * u_transform * vec4(a_pos, 1.0);"
  "}";

static const char* frag_src = ""
  "#version 330 core\n"
  "in vec2 v_tex;"
  "out vec4 o_color;"
  "void main() {"
  "  o_color = vec4(v_tex, 0.5, 1.0);"
  "}";

EntityShader::EntityShader() : Shader(&vert_src, &frag_src) {
  u_transform = glGetUniformLocation(shader_program, "u_transform");
  u_view_projection = glGetUniformLocation(shader_program, "u_view_projection");
}

void EntityShader::SetTransform(const glm::mat4& transform) {
  Bind();
  glUniformMatrix4fv(u_transform, 1, GL_FALSE, glm::value_ptr(transform));
}

void EntityShader::SetViewProjection(const glm::mat4& view_projection) {
  Bind();
  glUniformMatrix4fv(u_view_projection, 1, GL_FALSE, glm::value_ptr(view_projection));
}

}
