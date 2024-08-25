#pragma once

namespace Slap42 {
namespace Noise {

void SetSeed(int seed);
int GetSeed();
float SampleHeight(float x, float z);
float SampleTexture(float x, float z);

}
}
