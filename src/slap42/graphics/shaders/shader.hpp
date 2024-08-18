#pragma once

namespace Slap42 {

class Shader {
public:
  Shader(const char* vert_path, const char* frag_path);
  virtual ~Shader();

public:
  void Bind() const;
  
protected:
  unsigned int shader_program;
};

}
