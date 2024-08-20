#include "terrain_shader.hpp"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace Slap42 {
namespace Shader {
namespace TerrainShader {

static unsigned int shader_program;
static unsigned int u_view_projection;

void Create() {
  shader_program = Compile("res/shaders/terrain_shader.vert.glsl", "res/shaders/terrain_shader.frag.glsl");
  u_view_projection = glGetUniformLocation(shader_program, "u_view_projection");
  glUniform1i(glGetUniformLocation(shader_program, "u_texture_grass"), 0);
  glUniform1i(glGetUniformLocation(shader_program, "u_texture_dirt"), 1);
}

void Destroy() {
  glDeleteProgram(shader_program);
}

void Bind() {
  glUseProgram(shader_program);
}

void SetViewProjection(const glm::mat4& view_projection) {
  Bind();
  glUniformMatrix4fv(u_view_projection, 1, GL_FALSE, glm::value_ptr(view_projection));
}

}
}
}
