#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace Slap42 {
namespace MeshGen {

struct RawMesh {
  std::vector<float> vertices;
  std::vector<uint16_t> indices;
  size_t current_index = 0;
};

}
}
