#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex;

out vec2 v_tex;

uniform mat4 u_transform;
uniform mat4 u_view_projection;

void main() {
  v_tex = a_tex;
  gl_Position = u_view_projection * u_transform * vec4(a_pos, 1.0);
}
