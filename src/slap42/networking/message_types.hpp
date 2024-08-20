#pragma once

#include <string>
#include <bytepack/bytepack.hpp>
#include <glm/glm.hpp>

namespace Slap42 {

enum class MessageType : uint8_t {
  kNull                = 0x00,
  kPositionUpdate      = 0x01,
  kPlayerJoin          = 0x02,
  kPlayerLeave         = 0x03,
  kChatMessage         = 0x04,
};

typedef uint8_t peer_id;

struct PlayerPositionUpdateMessage {
  peer_id id;
  glm::vec3 pos;
  glm::vec2 rot;

  MessageType Type() const { return MessageType::kPositionUpdate; }

  void serialize(bytepack::binary_stream<>& stream) const {
    stream.write(id, pos.x, pos.y, pos.z, rot.x, rot.y);
  }

  void deserialize(bytepack::binary_stream<>& stream) {
    stream.read(id, pos.x, pos.y, pos.z, rot.x, rot.y);
  }
};

struct PlayerJoinMessage {
  peer_id id;
  glm::vec3 pos;
  glm::vec2 rot;

  MessageType Type() const { return MessageType::kPlayerJoin; }

  void serialize(bytepack::binary_stream<>& stream) const {
    stream.write(id, pos.x, pos.y, pos.z, rot.x, rot.y);
  }

  void deserialize(bytepack::binary_stream<>& stream) {
    stream.read(id, pos.x, pos.y, pos.z, rot.x, rot.y);
  }
};

struct PlayerLeaveMessage {
  peer_id id;

  MessageType Type() const { return MessageType::kPlayerLeave; }

  void serialize(bytepack::binary_stream<>& stream) const {
    stream.write(id);
  }

  void deserialize(bytepack::binary_stream<>& stream) {
    stream.read(id);
  }
};

struct ChatMessageMessage {
  peer_id id;
  std::string msg;

  MessageType Type() const { return MessageType::kChatMessage; }

  void serialize(bytepack::binary_stream<>& stream) const {
    stream.write(id, msg);
  }

  void deserialize(bytepack::binary_stream<>& stream) {
    stream.read(id, msg);
  }
};

}
