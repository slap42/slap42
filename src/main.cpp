#include <cstdio>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

const char* vert_src = ""
  "#version 330 core\n"
  "layout (location = 0) in vec3 a_pos;"
  "out vec3 v_pos;"
  "void main() {"
  "  gl_Position = vec4(a_pos, 1.0);"
  "  v_pos = a_pos + 0.5;"
  "}";
const char* frag_src = ""
  "#version 330 core\n"
  "in vec3 v_pos;"
  "out vec4 o_color;"
  "void main() {"
  "  o_color = vec4(v_pos, 1.0);"
  "}";

static GLuint CreateShaderModule(GLenum type, const char** code) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, code, nullptr);
  glCompileShader(shader);

  int success;
  char info_log[512] { };
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, sizeof(info_log), nullptr, info_log);
    fprintf(stderr, "Shader compile failed with error: %s\n", info_log);
  }

  return shader;
}

int main() {
  if (!glfwInit()) {
    fprintf(stderr, "glfwInit failed\n");
    return -1;
  }
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  GLFWwindow* window = glfwCreateWindow(1280, 720, "Test", nullptr, nullptr);
  if (!window) {
    fprintf(stderr, "glfwCreateWindow failed\n");
    return -1;
  }

  glfwMakeContextCurrent(window);
  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    fprintf(stderr, "gladLoadGl failed\n");
    return -1;
  }

  GLuint vert_shader = CreateShaderModule(GL_VERTEX_SHADER, &vert_src);
  GLuint frag_shader = CreateShaderModule(GL_FRAGMENT_SHADER, &frag_src);
  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vert_shader);
  glAttachShader(shader_program, frag_shader);
  glLinkProgram(shader_program);

  int success;
  char info_log[512] { };
  glGetShaderiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_program, sizeof(info_log), nullptr, info_log);
    fprintf(stderr, "Shader link failed with error: %s\n", info_log);
  }

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
  glUseProgram(shader_program);

  glClearColor(0.2, 0.4, 0.6, 1.0);

  GLuint vao, vbo, ibo;
  float vertices[] = {
    -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
  };
  unsigned short indices[] = {
    0, 1, 2,
    2, 1, 3,
  };

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ibo);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,  GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
