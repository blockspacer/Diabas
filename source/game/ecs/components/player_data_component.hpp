#ifndef PLAYER_DATA_COMPONENT_HPP_
#define PLAYER_DATA_COMPONENT_HPP_

#include <alflib/memory/raw_memory_writer.hpp>
#include <alflib/memory/raw_memory_reader.hpp>
#include "core/types.hpp"
#include "core/uuid.hpp"
#include "network/connection_id.hpp"
#include "core/value_store.hpp"

namespace dib {


struct PlayerData
{
  Uuid uuid;
  // Note: connection_id will not be serialized
  ConnectionId connection_id;
  u16 ping;
  u8 con_quality_local;
  u8 con_quality_remote;

  String name;
  u64 xp;
  u8 player_class;
  u8 body;
  u8 direction;
  float speed;

  ValueStore dynamic_state{};

  bool operator==(const PlayerData& other) const { return uuid == other.uuid; }

  bool operator!=(const PlayerData& other) const
  {
    return !(operator==(other));
  }

  bool ToBytes(alflib::RawMemoryWriter& mw) const
  {
    mw.Write(uuid);
    mw.Write(ping);
    mw.Write(con_quality_local);
    mw.Write(con_quality_remote);
    mw.Write(name);
    mw.Write(xp);
    mw.Write(player_class);
    mw.Write(body);
    mw.Write(direction);
    mw.Write(speed);
    return mw.Write(dynamic_state);
  }

  static PlayerData FromBytes(alflib::RawMemoryReader& mr)
  {
    PlayerData data{};
    data.uuid = mr.Read<decltype(uuid)>();
    data.ping = mr.Read<decltype(ping)>();
    data.con_quality_local = mr.Read<decltype(con_quality_local)>();
    data.con_quality_remote = mr.Read<decltype(con_quality_remote)>();
    data.name = mr.Read<decltype(name)>();
    data.xp = mr.Read<decltype(xp)>();
    data.player_class = mr.Read<decltype(player_class)>();
    data.body = mr.Read<decltype(body)>();
    data.direction = mr.Read<decltype(direction)>();
    data.speed = mr.Read<decltype(speed)>();
    data.dynamic_state = mr.Read<decltype(dynamic_state)>();
    return data;
  }

  String ToString() const
  {
    String str{};
    str += "name: " + name + ", ";
    str += "uuid: " + uuid.ToString();
    return str;
  }

  inline friend std::ostream& operator<<(std::ostream& os,
                                         const PlayerData& player_data)
  {
    return os << player_data.ToString();
  }
};
}

#endif // PLAYER_DATA_COMPONENT_HPP_
