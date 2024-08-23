#pragma once

struct GLFWwindow;

namespace Slap42 {
namespace Window {

GLFWwindow* GetGlfwWindow();
void Create();
void Destroy();
bool IsOpen();
void PollEvents();
void Clear();
void SwapBuffers();
void ImguiBegin();
void ImguiEnd();
int GetFramebufferWidth();
int GetFramebufferHeight();

}
}
