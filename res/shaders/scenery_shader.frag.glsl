#version 330 core

in vec2 v_tex;
in vec3 v_norm;

out vec4 o_color;

uniform sampler2DArray u_texture_array;

uniform vec3 u_sun_direction;
const float ambient = 0.2;

float CalculateLight() {
  return max(-dot(v_norm, u_sun_direction), ambient);
}

void main() {
  o_color = texture(u_texture_array, vec3(v_tex, 2.0)) * CalculateLight();
}
