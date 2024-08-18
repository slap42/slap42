#version 330 core

in vec2 v_tex;

out vec4 o_color;

void main() {
  o_color = vec4(v_tex, 0.5, 1.0);
}
