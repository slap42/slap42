#include "hostname_port_validation.hpp"

#include <sstream>
#include "menus/error_menu.hpp"

namespace Slap42 {
namespace Validation {

static int stoi_wrapper(const std::string& str, int* p_value, std::size_t* pos = 0, int base = 10) {
  try {
    *p_value = std::stoi(str, pos, base);
    return 0;
  }
  
  catch (const std::invalid_argument& ia) {
    return -1;
  }
  
  catch (const std::out_of_range& oor) {
    return -2;
  }
  
  catch (const std::exception& e) {
    return -3;
  }
}

bool TryParsePort(std::string buf, uint16_t& port) {
  int p;
  if (stoi_wrapper(buf, &p)) {
    std::stringstream ss;
    ss << buf << " is not a valid port number";
    ErrorMenu::SetErrorMessage(ss.str().c_str());
    return false;
  }
  if (p > std::numeric_limits<uint16_t>::max()) {
    ErrorMenu::SetErrorMessage("Port number can't be outside the range 0-65535");
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
