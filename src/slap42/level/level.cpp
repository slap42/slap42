#include "level.hpp"

#include "utils/hash.hpp"
#include "graphics/camera.hpp"

namespace Slap42 {

// Minimum value: 3
static int render_distance = 8;

Level::Level() {
  Shader::TerrainShader::Create();

  grass_texture = new Texture("res/images/Grass002_2K-PNG_Color.png");
  dirt_texture = new Texture("res/images/Ground067_2K-PNG_Color.png");
}

Level::~Level() {
  for (const auto& chunk : chunks) {
    delete chunk.second;
  }

  delete grass_texture;
  delete dirt_texture;
  
  Shader::TerrainShader::Destroy();
}

void Level::Update() {
  const glm::vec3 player_pos = Camera::GetPosition();
  
  const int kChunkDespawnDistance = render_distance * 2;
  glm::vec2 player_chunk_pos = { -((player_pos.x + Chunk::kChunkSize / 2) / Chunk::kChunkSize), -((player_pos.z + Chunk::kChunkSize / 2) / Chunk::kChunkSize) };

  // Load chunks close to the player
  for (int x = player_chunk_pos.x - render_distance; x < player_chunk_pos.x + render_distance; ++x) {
    for (int z = player_chunk_pos.y - render_distance; z < player_chunk_pos.y + render_distance; ++z) {

      glm::vec2 chunk_pos = { x, z };
      if (glm::distance(chunk_pos, player_chunk_pos) > render_distance) continue;

      uint64_t hash = FakeHash(x, z);
      if (chunks.find(hash) == chunks.end()) {
        Chunk* chunk = new Chunk(x, z);
        chunks.emplace(hash, chunk);
        goto break_loop;
      }

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

void Level::Render() const {
  Shader::TerrainShader::Bind();
  
  grass_texture->Bind(0);
  dirt_texture->Bind(1);

  for (const auto& chunk : chunks) {
    chunk.second->Render();
  }
}

int* Level::GetRenderDistancePtr() {
  return &render_distance;
}

}
