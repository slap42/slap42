#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include "chunk.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture_array.hpp"

namespace Slap42 {

class Level {
public:
  Level();
  ~Level();

public:
  void Update(const glm::vec3& player_pos);
  void Render() const;

public:
  inline Shader* GetShader() const { return shader; }

private:
  Shader* shader;
  TextureArray* textures;

  std::unordered_map<uint64_t, Chunk*> chunks;
};

}
