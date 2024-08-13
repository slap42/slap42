#pragma once

namespace Slap42 {
namespace ServerPanel {

void Render();

void OnPlayerJoin(const char* username);
void OnPlayerLeave(const char* username);

}
}
