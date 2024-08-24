#pragma once

#include <bytepack/bytepack.hpp>
#include <glm/glm.hpp>

namespace Slap42 {

enum class MessageType : uint8_t {
  kNull                = 0x00,
  kPositionUpdate      = 0x01,
  kPlayerJoin          = 0x02,
  kPlayerLeave         = 0x03,
  kChatMessage         = 0x04,
  kKickPlayer          = 0x05,
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
  bool kicked = false;

  MessageType Type() const { return MessageType::kPlayerLeave; }

  void serialize(bytepack::binary_stream<>& stream) const {
    stream.write(id, kicked);
  }

  void deserialize(bytepack::binary_stream<>& stream) {
    stream.read(id, kicked);
  }
};

struct ChatMessageMessage {
  peer_id id;
  char msg_buf[256] { };

  MessageType Type() const { return MessageType::kChatMessage; }

  void serialize(bytepack::binary_stream<>& stream) const {
    stream.write(id, msg_buf);
  }

  void deserialize(bytepack::binary_stream<>& stream) {
    stream.read(id, msg_buf);
  }
};

struct KickPlayerMessage {
  peer_id id;
  
  MessageType Type() const { return MessageType::kKickPlayer; }

  void serialize(bytepack::binary_stream<>& stream) const {
    stream.write(id);
  }

  void deserialize(bytepack::binary_stream<>& stream) {
    stream.read(id);
  }
};

}
