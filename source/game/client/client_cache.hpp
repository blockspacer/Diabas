#pragma once

// ========================================================================== //
// Headers
// ========================================================================== //

#include <alflib/graphics/image_atlas.hpp>

#include "core/macros.hpp"
#include "core/types.hpp"
#include "game/item/item_registry.hpp"
#include "game/tile/tile_registry.hpp"
#include "game/wall/wall_registry.hpp"
#include "graphics/texture.hpp"

// ========================================================================== //
// ClientCache Declaration
// ========================================================================== //

namespace dib::game {

/** Class that represents a cache of client-specific data. Such as the tile and
 * item atlas textures **/
class ClientCache
{
public:
  /** Atlas region **/
  struct AtlasRegion
  {
    /** Position **/
    u32 x, y;
    /** Width and height **/
    u32 w, h;
  };

private:
  /** Tile atlas **/
  alflib::ImageAtlas<> mTileAtlas;
  /** List of regions for tile resources. The outer index is the tile ID and the
   * inner index is the resource index **/
  std::vector<std::vector<AtlasRegion>> mTileResources;
  /** Tile atlas texture **/
  std::shared_ptr<graphics::Texture> mTileAtlasTexture;
  /** Inverse Tile atlas size **/
  Vector2F mInverseTileAtlasSize;

  /** Wall atlas **/
  alflib::ImageAtlas<> mWallAtlas;
  /** List of regions for wall resources. The outer index is the wall ID and the
   * inner index is the resource index **/
  std::vector<std::vector<AtlasRegion>> mWallResources;
  /** Wall atlas texture **/
  std::shared_ptr<graphics::Texture> mWallAtlasTexture;
  /** Inverse wall atlas size **/
  Vector2F mInverseWallAtlasSize;

  /** Item atlas **/
  alflib::ImageAtlas<> mItemAtlas;
  /** List of regions for item resources. The outer index is the item ID and the
   * inner index is the resource index **/
  std::vector<std::vector<AtlasRegion>> mItemResources;
  /** Item atlas texture **/
  std::shared_ptr<graphics::Texture> mItemAtlasTexture;
  /** Inverse Item atlas size **/
  Vector2F mInverseItemAtlasSize;

public:
  /** Construct a client cache **/
  ClientCache() = default;

  /** Build the tile atlas **/
  void BuildTileAtlas();

  /** Build the wall atlas **/
  void BuildWallAtlas();

  /** Build the item atlas **/
  void BuildItemAtlas();

  /** Returns the list of sub-resources for a tile ID **/
  const std::vector<AtlasRegion>& GetTileSubResources(TileRegistry::TileID id);

  /** Returns the number of item sub-resources for an item ID **/
  u32 GetTileSubResourceCount(TileRegistry::TileID id);

  /** Retrieves the texture coordinates for a tile ID with a given sub-resource
   * index **/
  void GetTextureCoordinatesForTile(TileRegistry::TileID id,
                                    u32 resourceIndex,
                                    Vector2F& texMin,
                                    Vector2F& texMax);

  /** Return the tile atlas texture **/
  [[nodiscard]] const std::shared_ptr<graphics::Texture>& GetTileAtlasTexture()
    const
  {
    return mTileAtlasTexture;
  }

  /** Returns the list of sub-resources for a wall ID **/
  const std::vector<AtlasRegion>& GetWallSubResources(WallRegistry::WallID id);

  /** Returns the number of wall sub-resources for a wall ID **/
  u32 GetWallSubResourceCount(WallRegistry::WallID id);

  /** Retrieves the texture coordinates for a wall ID with a given sub-resource
   * index **/
  void GetTextureCoordinatesForWall(WallRegistry::WallID id,
                                    u32 resourceIndex,
                                    Vector2F& texMin,
                                    Vector2F& texMax);

  /** Return the wall atlas texture **/
  [[nodiscard]] const std::shared_ptr<graphics::Texture>& GetWallAtlasTexture()
    const
  {
    return mWallAtlasTexture;
  }

  /** Returns the list of sub-resources for an item ID **/
  const std::vector<AtlasRegion>& GetItemSubResources(ItemRegistry::ItemID id);

  /** Returns the number of item sub-resources for an item ID **/
  u32 GetItemSubResourceCount(ItemRegistry::ItemID id);

  /** Retrieves the texture coordinates for an item ID with a given sub-resource
   * index **/
  void GetTextureCoordinatesForItem(ItemRegistry::ItemID id,
                                    u32 resourceIndex,
                                    Vector2F& texMin,
                                    Vector2F& texMax);

  /** Return the item atlas texture **/
  [[nodiscard]] const std::shared_ptr<graphics::Texture>& GetItemAtlasTexture()
    const
  {
    return mItemAtlasTexture;
  }

private:
  static String CreateAtlasKey(const String& type,
                               u32 resourceIndex,
                               u32 subResourceIndex);
};

}