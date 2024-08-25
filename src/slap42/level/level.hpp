#pragma once

#include <cstdint>

namespace Slap42 {
namespace Level {

void Create();
void Destroy();
void UnloadChunks();

void Update();
void Render();

void SetSeed(uint32_t seed);

int GetRenderDistance();
void SetRenderDistance(int render_distance);

}
}
