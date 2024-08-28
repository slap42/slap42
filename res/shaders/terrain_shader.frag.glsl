#version 330 core

in vec2  v_tex;
in float v_tex_blend;
in vec3  v_norm;

out vec4 o_color;

uniform sampler2DArray u_texture_array;

uniform vec3 u_sun_direction;
const float ambient = 0.2;

float CalculateLight() {
  return max(dot(v_norm, u_sun_direction), ambient);
}

void main() {
  int tex0 = int(v_tex_blend);
  int tex1 = tex0 + 1;

  o_color = mix(
    texture(u_texture_array, vec3(v_tex, tex0)),
    texture(u_texture_array, vec3(v_tex, tex1)),
    v_tex_blend - float(tex0)
  )
  * CalculateLight();
}
