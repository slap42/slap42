#include <sstream>

namespace Slap42 {
namespace Validation {

bool TryStoi(const std::string& str, int* p_value, std::size_t* pos, int base) {
  try {
    *p_value = std::stoi(str, pos, base);
    return true;
  }
  
  catch (const std::invalid_argument& ia) {
    return false;
  }
  
  catch (const std::out_of_range& oor) {
    return false;
  }
  
  catch (const std::exception& e) {
    return false;
  }
}

bool TryStou(const std::string& str, uint32_t* p_value, std::size_t* pos, int base) {
  try {
    *p_value = (uint32_t)std::stoul(str, pos, base);
    return true;
  }
  
  catch (const std::invalid_argument& ia) {
    return false;
  }
  
  catch (const std::out_of_range& oor) {
    return false;
  }
  
  catch (const std::exception& e) {
    return false;
  }
}

}
}
