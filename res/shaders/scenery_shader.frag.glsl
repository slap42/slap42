#version 330 core

in vec2 v_tex;

out vec4 o_color;

uniform sampler2DArray u_texture_array;

void main() {
  o_color = texture(u_texture_array, vec3(v_tex, 2.0));
}
