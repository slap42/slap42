#pragma once

struct GLFWwindow;

namespace Slap42 {
namespace Window {

GLFWwindow* GetWindow();
void CreateWindow();
void DestroyWindow();
bool IsWindowOpen();
void PollWindowEvents();
void ClearWindow();
void SwapBuffers();
void ImguiBegin();
void ImguiEnd();

}
}
