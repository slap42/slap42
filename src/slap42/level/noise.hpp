#pragma once

namespace Slap42 {
namespace Noise {

void SetSeed(int seed);
int GetSeed();
float SampleTerrainHeight(float x, float z);
float SampleTerrainTexture(float x, float z);
bool SampleTrees(float x, float z);

}
}
