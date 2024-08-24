#pragma once

#include <enet/enet.h>
#include <glm/glm.hpp>

namespace Slap42 {
  
struct PeerData {
  glm::vec3 pos;
  glm::vec2 rot;
  ENetPeer* peer;
};

}
