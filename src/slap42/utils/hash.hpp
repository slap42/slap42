#pragma once

#include <cstdint>
#include <cstring>

namespace Slap42 {

static uint64_t FakeHash(int x, int z) {
  uint64_t hash;
  int nums[2] { x, z };
  std::memcpy(&hash, nums, 2 * sizeof(int));
  return hash;
}

}
