#version 330 core
in vec2 v_tex;
in float v_tex_blend;

out vec4 o_color;

uniform sampler2D u_texture[3];

void main() {
  int tex0 = int(v_tex_blend);
  int tex1 = tex0 + 1;

  o_color = mix(
    texture(u_texture[tex0], v_tex),
    texture(u_texture[tex1], v_tex),
    v_tex_blend - float(tex0)
  );
}
