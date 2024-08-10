#include <cstdio>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/mesh.hpp"
#include "graphics/shader.hpp"

int main() {
  using namespace Slap42;

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

  glm::mat4 projection = glm::mat4(1.0f);
  glfwSetWindowUserPointer(window, &projection);
  auto on_resize = [](GLFWwindow* window, int width, int height) {
    glm::mat4* projection = (glm::mat4*)glfwGetWindowUserPointer(window);
    *projection = glm::perspectiveFov(3.14f / 2.0f, (float)width, float(height), 0.1f, 1000.0f);
    glViewport(0, 0, width, height);
  };
  glfwSetWindowSizeCallback(window, on_resize);
  on_resize(window, 1280, 720);

  glClearColor(0.2, 0.4, 0.6, 1.0);

  Shader shader;
  shader.Bind();

  float vertices[] = {
    -0.5f,  0.5f, -1.0f,
     0.5f,  0.5f, -1.0f,
    -0.5f, -0.5f, -1.0f,
     0.5f, -0.5f, -1.0f,
  };
  unsigned short indices[] = {
    0, 1, 2,
    2, 1, 3,
  };
  Mesh mesh(vertices, sizeof(vertices), indices, sizeof(indices));

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);

    shader.SetViewProjection(projection);
    mesh.Render();

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
