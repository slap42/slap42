#pragma once

#include <glm/glm.hpp>
#include "shaders/entity_shader.hpp"
#include "shaders/terrain_shader.hpp"

namespace Slap42 {

class Camera {
public:
  Camera(EntityShader* entity_shader, TerrainShader* terrain_shader);

public:
  void Update();

public:
  void OnResize(int width, int height);

private:
  void CalcView();
  void CalcViewProjection();

public:
  inline glm::vec3 GetPosition() const { return position; }

private:
  // Owned stuff
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec2 rotation = glm::vec2(0.0f);

  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  // Non-owned pointers
  EntityShader* entity_shader;
  TerrainShader* terrain_shader;
};

}
