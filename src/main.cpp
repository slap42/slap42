#include <GLFW/glfw3.h>

int main() {
  glfwInit();
  GLFWwindow* window = glfwCreateWindow(1280, 720, "Test", nullptr, nullptr);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
