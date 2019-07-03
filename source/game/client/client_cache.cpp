#include "game/client/client_cache.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

#include <dlog.hpp>

#include "game/constants.hpp"
#include "game/tile/tile_registry.hpp"

// ========================================================================== //
// ClientCache Implementation
// ========================================================================== //

namespace dib::game {

void
ClientCache::BuildTileAtlas(const TileRegistry& tileRegistry)
{
  // List of all images to pack and their keys
  alflib::ArrayList<alflib::Image> packImages;
  alflib::ArrayList<String> packNames;

  // Go through each registered tile
  u32 tileIndex = 0;
  for (const Tile* tile : tileRegistry.GetTiles()) {
    // Load the resource
    const ResourcePath& resourcePath = tile->GetResourcePath();
    alflib::Image resourceImage;
    resourceImage.Load(resourcePath.GetPath());

    // Add image for each sub-resource
    u32 resourceCountX = resourceImage.GetWidth() / TILE_SIZE;
    u32 resourceCountY = resourceImage.GetHeight() / TILE_SIZE;
    u32 tileSubIndex = 0;
    for (u32 y = 0; y < resourceCountY; y++) {
      for (u32 x = 0; x < resourceCountX; x++) {
        alflib::Image subResourceImage;
        subResourceImage.Create(TILE_SIZE, TILE_SIZE);
        subResourceImage.Blit(resourceImage,
                              0,
                              0,
                              x * TILE_SIZE,
                              y * TILE_SIZE,
                              TILE_SIZE,
                              TILE_SIZE);
        packImages.AppendEmplace(std::move(subResourceImage));
        packNames.AppendEmplace(
          CreateAtlasKey("tile", tileIndex, tileSubIndex));
        tileSubIndex++;
      }
    }
    std::vector<AtlasRegion> resourceVector;
    resourceVector.resize(tileSubIndex);
    mTileResources.emplace_back(std::move(resourceVector));
    tileIndex++;
  }

  // Create atlas
  u32 atlasDimension =
    u32(std::ceil(alflib::SquareRoot(f32(packImages.GetSize())))) * TILE_SIZE;
  mTileAtlas.Build(packImages, packNames, atlasDimension, atlasDimension);
  alflib::Image::Result result =
    mTileAtlas.GetImage().Save(Path{ "./res/tiles/atlas.tga" }, true);
  if (result != alflib::Image::Result::kSuccess) {
    DLOG_WARNING("Failed to save item atlas (the game will still work)");
  }

  // Retrieve and store regions in list
  for (u32 index = 0; index < tileRegistry.GetTiles().size(); index++) {
    u32 subResourceCount = mTileResources[index].size();
    for (u32 subIndex = 0; subIndex < subResourceCount; subIndex++) {
      AtlasRegion& region = mTileResources[index][subIndex];

      String key = CreateAtlasKey("tile", index, subIndex);
      alflib::ImageAtlasRegion imageRegion = mTileAtlas.GetRegion(key);
      region.x = imageRegion.x;
      region.y = imageRegion.y;
      region.w = imageRegion.width;
      region.h = imageRegion.height;
    }
  }

  // Create atlas texture
  mTileAtlasTexture = std::make_shared<graphics::Texture>("TileAtlas");
  mTileAtlasTexture->Load(mTileAtlas.GetImage());
}

// -------------------------------------------------------------------------- //

void
ClientCache::BuildItemAtlas(const ItemRegistry& itemRegistry)
{
  // List of all images to pack and their keys
  alflib::ArrayList<alflib::Image> packImages;
  alflib::ArrayList<String> packNames;

  // Go through each registered item
  u32 itemIndex = 0;
  for (const Item* item : itemRegistry.GetItems()) {
    // Load the resource
    const ResourcePath& resourcePath = item->GetResourcePath();
    alflib::Image resourceImage;
    resourceImage.Load(resourcePath.GetPath());

    // Add image for each sub-resource
    const Vector2I& resourceCount = item->GetSubResourceCount();
    Vector2I resourceSize(resourceImage.GetWidth() / resourceCount.x,
                          resourceImage.GetHeight() / resourceCount.y);

    u32 itemSubIndex = 0;
    for (u32 y = 0; y < u32(resourceCount.y); y++) {
      for (u32 x = 0; x < u32(resourceCount.x); x++) {
        alflib::Image subResourceImage;
        subResourceImage.Create(resourceSize.x, resourceSize.y);
        subResourceImage.Blit(resourceImage,
                              0,
                              0,
                              x * resourceSize.x,
                              y * resourceSize.y,
                              resourceSize.x,
                              resourceSize.y);
        packImages.AppendEmplace(std::move(subResourceImage));
        packNames.AppendEmplace(
          CreateAtlasKey("item", itemIndex, itemSubIndex));
        itemSubIndex++;
      }
    }
    std::vector<AtlasRegion> resourceVector;
    resourceVector.resize(itemSubIndex);
    mItemResources.emplace_back(std::move(resourceVector));
    itemIndex++;
  }

  // Create atlas
  u32 atlasDimension = 256;
  while (
    mItemAtlas.Build(packImages, packNames, atlasDimension, atlasDimension) !=
    alflib::ImageAtlas<>::Result::kSuccess) {
    atlasDimension *= 2;
  }
  alflib::Image::Result result =
    mItemAtlas.GetImage().Save(Path{ "./res/items/atlas.tga" }, true);
  if (result != alflib::Image::Result::kSuccess) {
    DLOG_WARNING("Failed to save item atlas (the game will still work)");
  }

  // Retrieve and store regions in list
  for (u32 index = 0; index < itemRegistry.GetItems().size(); index++) {
    u32 subResourceCount = mItemResources[index].size();
    for (u32 subIndex = 0; subIndex < subResourceCount; subIndex++) {
      AtlasRegion& region = mItemResources[index][subIndex];

      String key = CreateAtlasKey("item", index, subIndex);
      alflib::ImageAtlasRegion imageRegion = mItemAtlas.GetRegion(key);
      region.x = imageRegion.x;
      region.y = imageRegion.y;
      region.w = imageRegion.width;
      region.h = imageRegion.height;
    }
  }

  // Create atlas texture
  mItemAtlasTexture = std::make_shared<graphics::Texture>("ItemAtlas");
  mItemAtlasTexture->Load(mItemAtlas.GetImage());
}

// -------------------------------------------------------------------------- //

const std::vector<ClientCache::AtlasRegion>&
ClientCache::GetSubResources(TileRegistry::TileID id)
{
  return mTileResources[id];
}

// -------------------------------------------------------------------------- //

void
ClientCache::GetTextureCoordinatesForTile(TileRegistry::TileID id,
                                          u32 resourceIndex,
                                          Vector2F& texMin,
                                          Vector2F& texMax)
{
  AtlasRegion& region = mTileResources[id][resourceIndex];
  texMin = Vector2F(f32(region.x) / mTileAtlas.GetWidth(),
                    f32(region.y) / mTileAtlas.GetHeight());
  texMax = Vector2F(f32(region.x + region.w) / mTileAtlas.GetWidth(),
                    f32(region.y + region.h) / mTileAtlas.GetHeight());
}

// -------------------------------------------------------------------------- //

const std::vector<ClientCache::AtlasRegion>&
ClientCache::GetItemSubResources(ItemRegistry::ItemID id)
{
  return mItemResources[id];
}

// -------------------------------------------------------------------------- //

void
ClientCache::GetTextureCoordinatesForItem(ItemRegistry::ItemID id,
                                          u32 resourceIndex,
                                          Vector2F& texMin,
                                          Vector2F& texMax)
{
  AtlasRegion& region = mTileResources[id][resourceIndex];
  texMin = Vector2F(f32(region.x) / mTileAtlas.GetWidth(),
                    f32(region.y) / mTileAtlas.GetHeight());
  texMax = Vector2F(f32(region.x + region.w) / mTileAtlas.GetWidth(),
                    f32(region.y + region.h) / mTileAtlas.GetHeight());
}

// -------------------------------------------------------------------------- //

String
ClientCache::CreateAtlasKey(const String& type,
                            u32 resourceIndex,
                            u32 subResourceIndex)
{
  return type + "_" + String::ToString(resourceIndex) + "_" +
         String::ToString(subResourceIndex);
}

}