#include "hostname_port_validation.hpp"

#include <sstream>
#include "menus/host_error_menu.hpp"
#include "menus/join_error_menu.hpp"

namespace Slap42 {
namespace Validation {

bool TryParsePort(std::string buf, uint16_t& port) {
  for (char c : buf) {
    if (!isdigit(c)) {
      std::stringstream ss;
      ss << buf << " is not a valid port number";
      HostErrorMenu::SetErrorMessage(ss.str().c_str());
      return false;
    }
  }

  uint64_t p = std::stoi(buf);
  if (p > std::numeric_limits<uint16_t>::max()) {
    HostErrorMenu::SetErrorMessage("Port number can't be outside the range 0-65535");
    return false;
  }

  port = (uint16_t)p;
  return true;
}

bool TryParseHostAndPort(std::string buf, std::string& host, uint16_t& port) {
  size_t colon_pos = buf.find_first_of(':');

  if (colon_pos != std::string::npos) {
    // Colon found, try to parse host and port
    host = buf.substr(0, colon_pos);
    std::string port_str = buf.substr(colon_pos + 1);
    return TryParsePort(port_str, port);;
  }
  else {
    // No colon, use default port
    host = buf;
    port = 6969;
    return true;
  }
}

}
}
