#include "level.hpp"

#include <unordered_map>
#include "chunk.hpp"
#include "graphics/camera.hpp"
#include "graphics/shaders/terrain_shader.hpp"
#include "graphics/texture.hpp"
#include "utils/hash.hpp"

namespace Slap42 {
namespace Level {

static int render_distance = 8;
static Texture* grass_texture;
static Texture* dirt_texture;
static std::unordered_map<uint64_t, Chunk*> chunks;

void Create() {
  Shader::TerrainShader::Create();

  grass_texture = new Texture("res/images/Grass002_2K-PNG_Color.png");
  dirt_texture = new Texture("res/images/Ground067_2K-PNG_Color.png");
}

void Destroy() {
  UnloadChunks();

  delete grass_texture;
  delete dirt_texture;
  
  Shader::TerrainShader::Destroy();
}

void UnloadChunks() {
  for (const auto& chunk : chunks) {
    delete chunk.second;
  }
  chunks.clear();
}

void Update() {
  const glm::vec3 player_pos = Camera::GetPosition();
  
  const float kChunkSpawnDistance = render_distance * 0.4f;
  const float kChunkDespawnDistance = render_distance * 0.7f;
  glm::vec2 player_chunk_pos { 0.0f };
  if (player_pos.x > 0) {
    player_chunk_pos.x = -(player_pos.x / Chunk::kChunkSize) - 1;
  }
  else {
    player_chunk_pos.x = -(player_pos.x / Chunk::kChunkSize);
  }
  if (player_pos.z > 0) {
    player_chunk_pos.y = -(player_pos.z / Chunk::kChunkSize) - 1;
  }
  else {
    player_chunk_pos.y = -(player_pos.z / Chunk::kChunkSize);
  }

  // Load chunks close to the player
  {
    int x = 0, z = 0, dx = 0, dz = 0;
    dz = -1;
    int t = render_distance;
    int max_i = t * t;
    for (int i = 0; i < max_i; ++i) {
      // Load the chunk
      glm::vec2 chunk_pos = { x + player_chunk_pos.x, z + player_chunk_pos.y };

      if (glm::distance(chunk_pos, player_chunk_pos) <= kChunkSpawnDistance) {
        uint64_t hash = FakeHash(x + (int)player_chunk_pos.x, z + (int)player_chunk_pos.y);
        if (chunks.find(hash) == chunks.end()) {
          Chunk* chunk = new Chunk(x + (int)player_chunk_pos.x, z + (int)player_chunk_pos.y);
          chunks.emplace(hash, chunk);
          goto break_loop;
        }
      }

      if (x == z || (x < 0 && x == -z) || (x > 0 && x == 1 - z)) {
        t = dx;
        dx = -dz;
        dz = t;
      }

      x += dx;
      z += dz;
    }
  }
  break_loop:

  // Unload chunks far from the player
  for (auto it = chunks.begin(); it != chunks.cend();) {
    auto& chunk = *it;

    glm::vec2 chunk_pos = { chunk.second->GetX(), chunk.second->GetZ() };
    if (glm::distance(chunk_pos, player_chunk_pos) > kChunkDespawnDistance) {
      delete chunk.second;
      chunks.erase(it++);
    }
    else {
      ++it;
    }
  }
}

void Render() {
  Shader::TerrainShader::Bind();
  
  grass_texture->Bind(0);
  dirt_texture->Bind(1);

  for (const auto& chunk : chunks) {
    chunk.second->Render();
  }
}

int GetRenderDistance() {
  return render_distance;
}

void SetRenderDistance(int rd) {
  render_distance = rd;
}

}
}
