#pragma once

namespace Slap42 {

class Shader {
public:
  Shader();
  ~Shader();

public:
  void Bind() const;

private:
  unsigned int shader_program;
};

}
