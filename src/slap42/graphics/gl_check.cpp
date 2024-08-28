#include "gl_check.hpp"

#ifndef NDEBUG

#include <cstdio>
#include <cstdlib>
#include <glad/gl.h>

namespace Slap42 {

void CheckOpenGlError(const char* gl_statement, const char* file, int line) {
  int error = glGetError();

  if (error != GL_NO_ERROR) {
    fprintf(stderr, "OpenGl error %d\n\t%s\n\t%s:%d\n", error, gl_statement, file, line);
    exit(-1);
  }
}

}

#endif
