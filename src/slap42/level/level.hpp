#pragma once

namespace Slap42 {
namespace Level {

void Create();
void Destroy();
void UnloadChunks();

void Update();
void Render();

int GetRenderDistance();
void SetRenderDistance(int render_distance);

}
}
