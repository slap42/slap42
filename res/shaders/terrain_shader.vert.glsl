#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in float a_tex_blend;

out vec2 v_tex;
out float v_tex_blend;

uniform mat4 u_view_projection;

void main() {
  v_tex = vec2(a_pos.x / 32.0, a_pos.z / 32.0);
  v_tex_blend = a_tex_blend;
  gl_Position = u_view_projection * vec4(a_pos, 1.0);
}
