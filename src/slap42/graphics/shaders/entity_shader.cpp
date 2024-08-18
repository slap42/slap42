#include "entity_shader.hpp"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace Slap42 {

EntityShader::EntityShader() : Shader("res/shaders/entity_shader.vert.glsl", "res/shaders/entity_shader.frag.glsl") {
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
