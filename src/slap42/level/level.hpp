#pragma once

#include <cstdint>

namespace Slap42 {
namespace Level {

void Create();
void Destroy();
void UnloadChunks();

void Update(float delta);
void Render();

int GetRenderDistance();
void SetRenderDistance(int render_distance);

}
}
