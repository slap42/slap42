#pragma once

#include "menu_state.hpp"

namespace Slap42 {
namespace ErrorMenu {

void SetReturnMenu(MenuState b);
void SetErrorMessage(const char* msg);

void Render();

}
}
