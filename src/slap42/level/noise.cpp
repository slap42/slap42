#include "noise.hpp"

#include <memory>
#include "OpenSimplexNoise.hh"

namespace Slap42 {
namespace Noise {

static uint32_t seed = -1;
static std::unique_ptr<OSN::Noise<2>> noise_height;
constexpr static float kHeight = 100.0f;
constexpr static float kHeightSampleScale = 0.01f;

void SetSeed(int s) {
  seed = s;
  noise_height = std::make_unique<OSN::Noise<2>>(seed);
}

int GetSeed() {
  return seed;
}

// Returns a height value in the range 0-1
static float SampleHeightBase(float x, float z) {
  return ((noise_height->eval(x * kHeightSampleScale, z * kHeightSampleScale) + 1.0f) * 0.5f);
}

float SampleTerrainHeight(float x, float z) {
  return SampleHeightBase(x, z) * kHeight - kHeight * 0.5f;
}

float SampleTerrainTexture(float x, float z) {
  return SampleHeightBase(x, z);
}

}
}

