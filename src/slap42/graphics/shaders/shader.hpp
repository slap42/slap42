#pragma once

namespace Slap42 {

class Shader {
public:
  Shader(const char** vert_src, const char** frag_src);
  virtual ~Shader();

public:
  void Bind() const;
  
protected:
  unsigned int shader_program;
};

}
