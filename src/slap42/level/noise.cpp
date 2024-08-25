#include "noise.hpp"

#include <memory>
#include "OpenSimplexNoise.hh"

namespace Slap42 {
namespace Noise {

static uint32_t seed = -1;
static std::unique_ptr<OSN::Noise<2>> noise_height;
constexpr static float kHeight = 5.0f;
constexpr static float kHeightSampleScale = 0.1f;

void SetSeed(int s) {
  seed = s;
  noise_height = std::make_unique<OSN::Noise<2>>(seed);
}

int GetSeed() {
  return seed;
}

float SampleHeight(float x, float z) {
  return noise_height->eval(x * kHeightSampleScale, z * kHeightSampleScale) * kHeight;
}

float SampleTexture(float x, float z) {
  return noise_height->eval(x * kHeightSampleScale, z * kHeightSampleScale);
}

}
}

