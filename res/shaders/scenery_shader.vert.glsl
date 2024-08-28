#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex;
layout (location = 2) in vec3 a_norm;

out vec2 v_tex;
out vec3 v_norm;

uniform mat4 u_view_projection;

void main() {
  v_tex = a_tex;
  v_norm = a_norm;
  gl_Position = u_view_projection * vec4(a_pos, 1.0);
}
