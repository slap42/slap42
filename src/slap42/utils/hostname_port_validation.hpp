#pragma once

#include <cstdint>
#include <string>

namespace Slap42 {
namespace Validation {

bool TryParsePort(std::string buf, uint16_t& port);
bool TryParseHostAndPort(std::string buf, std::string& host, uint16_t& port);

}
}
