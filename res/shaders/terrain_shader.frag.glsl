#version 330 core

in vec2  v_tex;
in float v_tex_blend;
in vec3  v_norm;

out vec4 o_color;

uniform sampler2D u_texture[3];

const float ambient = 0.0;
const vec3 sun_dir = normalize(vec3(0.5, -1.0f, 0.0));

void main() {
  int tex0 = int(v_tex_blend);
  int tex1 = tex0 + 1;

  float diffuse = max(dot(v_norm, sun_dir), ambient);

  o_color = mix(
    texture(u_texture[tex0], v_tex),
    texture(u_texture[tex1], v_tex),
    v_tex_blend - float(tex0)
  )
  * diffuse;
}
