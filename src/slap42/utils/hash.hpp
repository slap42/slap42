#pragma once

#include <cstdint>
#include <cstring>

namespace Slap42 {

static uint64_t FakeHash(uint32_t x, uint32_t z) {
  uint64_t hash;
  uint32_t nums[2] { x, z };
  std::memcpy(&hash, nums, 2 * sizeof(uint32_t));
  return hash;
}

}
