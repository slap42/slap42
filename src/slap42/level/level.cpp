#include "level.hpp"

#include "utils/hash.hpp"

namespace Slap42 {

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

void Level::Update(const glm::vec3& player_pos) {
  // Minimum value: 3
  const int kChunkSpawnDistance = 8;
  const int kChunkDespawnDistance = kChunkSpawnDistance * 1.5;
  glm::vec2 player_chunk_pos = { -((player_pos.x + Chunk::kChunkSize / 2) / Chunk::kChunkSize), -((player_pos.z + Chunk::kChunkSize / 2) / Chunk::kChunkSize) };

  // Load chunks close to the player
  for (int x = player_chunk_pos.x - kChunkSpawnDistance; x < player_chunk_pos.x + kChunkSpawnDistance; ++x) {
    for (int z = player_chunk_pos.y - kChunkSpawnDistance; z < player_chunk_pos.y + kChunkSpawnDistance; ++z) {

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

}
