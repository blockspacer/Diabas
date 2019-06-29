#include "game/client/game_client.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

#include "app/client/imgui/imgui.h"
#include "game/client/world_renderer.hpp"
#include "game/client/debug_ui.hpp"

// ========================================================================== //
// Client Implementation
// ========================================================================== //

namespace dib::game {

GameClient::GameClient(const app::AppClient::Descriptor& descriptor)
  : AppClient(descriptor)
  , mModLoader(mScriptEnvironment, Path{ "./mods" })
  , mCamera(GetWidth(), GetHeight())
{
  mModLoader.Init(mWorld);
  mModLoader.RegisterTiles(mWorld.GetTileManager());
  mTileAtlas.Build(mWorld.GetTileManager());
  mWorld.GetTerrain().Generate();
}

// -------------------------------------------------------------------------- //

void
GameClient::Update(f64 delta)
{
  // ImGui
  if (ImGui::Begin("Diabas - Debug")) {
    ShowScriptDebug(*this);
    ShowModDebug(*this);
    ShowTileDebug(*this);
    ShowNetworkDebug(*this);
  }
  ImGui::End();

  UpdateCamera(delta);
  mWorld.Update();
}

// -------------------------------------------------------------------------- //

void
GameClient::Render()
{
  mRenderer.Clear(alflib::Color::CORNFLOWER_BLUE);

  WorldRenderer::RenderWorld(mRenderer, mTileAtlas, mWorld, mCamera);
}

// -------------------------------------------------------------------------- //

void
GameClient::UpdateCamera(f32 delta)
{
  // Move camera
  Vector3F cameraMove(0.0f, 0.0f, 0.0f);
  f32 cameraSpeed = 300;
  if (IsKeyDown(Key::kKeyLeft)) {
    cameraMove.x -= cameraSpeed * delta;
  }
  if (IsKeyDown(Key::kKeyRight)) {
    cameraMove.x += cameraSpeed * delta;
  }
  if (IsKeyDown(Key::kKeyUp)) {
    cameraMove.y += cameraSpeed * delta;
  }
  if (IsKeyDown(Key::kKeyDown)) {
    cameraMove.y -= cameraSpeed * delta;
  }
  mCamera.Move(cameraMove);

  Vector3F minPos = Vector3F(0.0f, 0.0f, 0.0f);
  Vector3F maxPos =
    Vector3F(mWorld.GetTerrain().GetWidth() * game::TileManager::TILE_SIZE -
               mCamera.GetWidth(),
             mWorld.GetTerrain().GetHeight() * game::TileManager::TILE_SIZE -
               mCamera.GetHeight(),
             1.0f);
  mCamera.ClampPosition(minPos, maxPos);
}

}