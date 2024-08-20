#include "entity_shader.hpp"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace Slap42 {
namespace Shader {
namespace EntityShader {

static unsigned int shader_program;
static unsigned int u_transform;
static unsigned int u_view_projection;

void Create() {
  shader_program = Compile("res/shaders/entity_shader.vert.glsl", "res/shaders/entity_shader.frag.glsl");
  u_transform = glGetUniformLocation(shader_program, "u_transform");
  u_view_projection = glGetUniformLocation(shader_program, "u_view_projection");
}

void Destroy() {
  glDeleteProgram(shader_program);
}

void Bind() {
  glUseProgram(shader_program);
}

void SetTransform(const glm::mat4& transform) {
  Bind();
  glUniformMatrix4fv(u_transform, 1, GL_FALSE, glm::value_ptr(transform));
}

void SetViewProjection(const glm::mat4& view_projection) {
  Bind();
  glUniformMatrix4fv(u_view_projection, 1, GL_FALSE, glm::value_ptr(view_projection));
}

}
}
}
