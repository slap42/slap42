#pragma once

#include <bytepack/bytepack.hpp>
#include <glm/glm.hpp>

namespace Slap42 {


enum class MessageType : uint8_t {
  kNull           = 0x00,
  kPositionUpdate = 0x01,
};

struct PositionUpdateMessage {
  glm::vec3 pos;
  glm::vec2 rot;

  void serialize(bytepack::binary_stream<>& stream) const {
    stream.write(pos.x, pos.y, pos.z, rot.x, rot.y);
  }

  void deserialize(bytepack::binary_stream<>& stream) {
    stream.read(pos.x, pos.y, pos.z, rot.x, rot.y);
  }
};

}
