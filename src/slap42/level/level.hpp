#pragma once

#include <unordered_map>
#include "chunk.hpp"
#include "graphics/shaders/terrain_shader.hpp"
#include "graphics/texture.hpp"

namespace Slap42 {

class Level {
public:
  Level();
  ~Level();

public:
  void Update();
  void Render() const;

public:
  static int GetRenderDistance();
  static void SetRenderDistance(int render_distance);

private:
  Texture* grass_texture;
  Texture* dirt_texture;

  std::unordered_map<uint64_t, Chunk*> chunks;
};

}
