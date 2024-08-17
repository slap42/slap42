#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include "chunk.hpp"
#include "graphics/shaders/terrain_shader.hpp"
#include "graphics/texture.hpp"

namespace Slap42 {

class Level {
public:
  Level();
  ~Level();

public:
  void Update(const glm::vec3& player_pos);
  void Render() const;

public:
  inline TerrainShader* GetShader() const { return terrain_shader; }

private:
  TerrainShader* terrain_shader;
  Texture* grass_texture;
  Texture* dirt_texture;

  std::unordered_map<uint64_t, Chunk*> chunks;
};

}
