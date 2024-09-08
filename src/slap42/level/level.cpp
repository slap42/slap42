#include "level.hpp"

#include <glad/gl.h>
#include <unordered_map>
#include "chunk.hpp"
#include "graphics/camera.hpp"
#include "graphics/shaders/scenery_shader.hpp"
#include "graphics/shaders/terrain_shader.hpp"
#include "graphics/texture_array.hpp"
#include "noise.hpp"
#include "utils/hash.hpp"

namespace Slap42 {
namespace Level {

static int render_distance = 16;
static TextureArray* texture_array;
static std::unordered_map<uint64_t, Chunk*> chunks;
static const char* kTextureNames[] = {
  "res/images/Grass002_2K-PNG_Color.png",
  "res/images/Ground067_2K-PNG_Color.png",
  "res/images/Bark014_2K-PNG_Color.png",
};
static glm::vec3 sun_dir;
static float sky_brightness;

void Create() {
  Shader::TerrainShader::Create();
  Shader::SceneryShader::Create();
  texture_array = new TextureArray(kTextureNames, sizeof(kTextureNames) / sizeof(char*));
}

void Destroy() {
  UnloadChunks();
  delete texture_array;
  Shader::SceneryShader::Destroy();
  Shader::TerrainShader::Destroy();
}

void UnloadChunks() {
  Noise::SetSeed(-1);
  
  for (const auto& chunk : chunks) {
    delete chunk.second;
  }
  chunks.clear();
}

void Update(float time, float delta) {
  // Update sun position and clear color for a day/night cycle
  // TODO: Get initial sun_animation_time from server and occasionally sync with other players
  float sun_animation_time = time * 0.00000027f + 2.0f; // Start in mid morning, 1 day/night cycle is approx 1 hour
  sun_dir = glm::vec3(std::sin(sun_animation_time), std::cos(sun_animation_time), 0.0f);
  sky_brightness = std::clamp(-std::cos(sun_animation_time) + 0.7f, 0.0f, 1.0f);
  
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
  if (Noise::GetSeed() != -1) {
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
  glClearColor(0.53f * sky_brightness, 0.81f * sky_brightness, 0.92f * sky_brightness, 1.0f);
  
  Shader::TerrainShader::Bind();
  Shader::TerrainShader::SetSunDirection(sun_dir);
  if (Camera::IsVpDirty()) {
    Shader::TerrainShader::SetViewProjection(Camera::GetViewProjection());
  }
  for (const auto& chunk : chunks) {
    chunk.second->RenderTerrain();
  }
  Shader::SceneryShader::Bind();
  Shader::SceneryShader::SetSunDirection(sun_dir);
  if (Camera::IsVpDirty()) {
    Shader::SceneryShader::SetViewProjection(Camera::GetViewProjection());
  }
  for (const auto& chunk : chunks) {
    chunk.second->RenderScenery();
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
