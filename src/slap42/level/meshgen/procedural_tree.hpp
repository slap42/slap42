#pragma once

#include <cstdint>
#include <vector>

namespace Slap42 {
namespace MeshGen {

void Tree(std::vector<float>& vertices, std::vector<uint16_t>& indices, size_t current_index, int xoffs, int zoffs);

}
}
