#version 330 core
in vec2 v_tex;
in float v_tex_blend;

out vec4 o_color;

uniform sampler2D u_texture_grass;
uniform sampler2D u_texture_dirt;

void main() {
  o_color = mix(texture(u_texture_grass, v_tex), texture(u_texture_dirt, v_tex), v_tex_blend);
}