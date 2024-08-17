#pragma once

#include <bytepack/bytepack.hpp>
#include <glm/glm.hpp>

namespace Slap42 {

enum class MessageType : uint8_t {
  kNull           = 0x00,
  kPositionUpdate = 0x01,
  kOnPlayerJoin   = 0x02,
  kOnPlayerLeave  = 0x03,
};

struct PositionUpdateMessage {
  glm::vec3 pos;
  glm::vec2 rot;
  uint32_t host;
  uint16_t port;

  MessageType Type() const { return MessageType::kPositionUpdate; }

  void serialize(bytepack::binary_stream<>& stream) const {
    stream.write(pos.x, pos.y, pos.z, rot.x, rot.y, host, port);
  }

  void deserialize(bytepack::binary_stream<>& stream) {
    stream.read(pos.x, pos.y, pos.z, rot.x, rot.y, host, port);
  }
};

struct OnPlayerJoinMessage {
  uint32_t host;
  uint16_t port;

  MessageType Type() const { return MessageType::kOnPlayerJoin; }

  void serialize(bytepack::binary_stream<>& stream) const {
    stream.write(host, port);
  }

  void deserialize(bytepack::binary_stream<>& stream) {
    stream.read(host, port);
  }
};

struct OnPlayerLeaveMessage {
  uint32_t host;
  uint16_t port;

  MessageType Type() const { return MessageType::kOnPlayerLeave; }

  void serialize(bytepack::binary_stream<>& stream) const {
    stream.write(host, port);
  }

  void deserialize(bytepack::binary_stream<>& stream) {
    stream.read(host, port);
  }
};

}
