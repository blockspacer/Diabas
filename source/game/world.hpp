#ifndef WORLD_HPP_
#define WORLD_HPP_

// ========================================================================== //
// Headers
// ========================================================================== //

#include <string_view>
#include <alflib/memory/memory_reader.hpp>
#include <alflib/memory/memory_writer.hpp>

#include "core/assert.hpp"
#include "network/network.hpp"
#include "network/side.hpp"
#include "game/ecs/entity_manager.hpp"
#include "game/terrain.hpp"
#include "game/chat/chat.hpp"
#include "game/tile/tile_registry.hpp"
#include "game/npc/npc_registry.hpp"

// ========================================================================== //
// World Declaration
// ========================================================================== //

namespace dib::game {

/** Class representing the game world **/
class World
{
public:
  World();

  World(World&& other);

  World& operator=(World&& other);

  void Update(f64 delta);

  void OnDisconnect();

  void OnCommandNetwork(const std::string_view input);

  void OnCommandBroadcast(const std::string_view input);

  Network<kSide>& GetNetwork() { return network_; }

  /** Save world to path **/
  bool Save(const Path& path, bool overwrite = false);

  /** Load world from path **/
  bool Load(const Path& path);

  /** Load world from memory reader **/
  bool Load(alflib::MemoryReader& reader);

  /** Returns the terrain of the world **/
  Terrain& GetTerrain() { return mTerrain; }

  /** Returns the terrain of the world **/
  const Terrain& GetTerrain() const { return mTerrain; }

  /** Returns the entity manager **/
  auto& GetEntityManager() { return entity_manager_; }
  const auto& GetEntityManager() const { return entity_manager_; }

  auto& GetChat() { return chat_; }
  const auto& GetChat() const { return chat_; }

  auto& GetNpcRegistry() { return npc_registry_; }
  const auto& GetNpcRegistry() const { return npc_registry_; }

  bool ToBytes(alflib::MemoryWriter& writer) const;

  bool& GetDrawAStar() { return draw_astar; }
  bool GetDrawAStar() const { return draw_astar; }

  f64 GetTime() const;

public:
  static World FromBytes(alflib::MemoryReader& reader);

private:
  /** Terrain **/
  Terrain mTerrain;

  EntityManager entity_manager_{};

  Network<kSide> network_{ this };

  Chat chat_{ this };

  NpcRegistry npc_registry_{};

  bool draw_astar = false;
};
}

#endif // WORLD_HPP_
