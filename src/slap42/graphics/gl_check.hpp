#pragma once

#ifndef NDEBUG

  namespace Slap42 {
    void CheckOpenGlError(const char* gl_statement, const char* file, int line);
  }

  #define GL_CHECK(x) do { \
    x; \
    ::Slap42::CheckOpenGlError(#x, __FILE__, __LINE__); \
  } while(0); 

#else

  #define GL_CHECK(x) x

#endif
