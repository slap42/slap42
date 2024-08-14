#include "entity_shader.hpp"

namespace Slap42 {

static const char* vert_src = ""
  "#version 330 core\n"
  "layout (location = 0) in vec3 a_pos;"
  "layout (location = 1) in vec2 a_tex;"
  "out vec2 v_tex;"
  "void main() {"
  "  v_tex = a_tex;"
  "  gl_Position = vec4(a_pos, 1.0);"
  "}";

static const char* frag_src = ""
  "#version 330 core\n"
  "in vec2 v_tex;"
  "out vec4 o_color;"
  "void main() {"
  "  o_color = vec4(v_tex, 0.5, 1.0);"
  "}";

EntityShader::EntityShader() : Shader(&vert_src, &frag_src) {
}

}
