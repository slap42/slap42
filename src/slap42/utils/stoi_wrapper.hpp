#pragma once

#include <cstdint>
#include <string>

namespace Slap42 {
namespace Validation {

bool TryStoi(const std::string& str, int* p_value, std::size_t* pos = 0, int base = 10);
bool TryStou(const std::string& str, uint32_t* p_value, std::size_t* pos = 0, int base = 10);

}
}
