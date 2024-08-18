#include "terrain_shader.hpp"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace Slap42 {

TerrainShader::TerrainShader() : Shader("res/shaders/terrain_shader.vert.glsl", "res/shaders/terrain_shader.frag.glsl") {
  u_view_projection = glGetUniformLocation(shader_program, "u_view_projection");
  glUniform1i(glGetUniformLocation(shader_program, "u_texture_grass"), 0);
  glUniform1i(glGetUniformLocation(shader_program, "u_texture_dirt"), 1);
}

void TerrainShader::SetViewProjection(const glm::mat4& view_projection) {
  Bind();
  glUniformMatrix4fv(u_view_projection, 1, GL_FALSE, glm::value_ptr(view_projection));
}

}
