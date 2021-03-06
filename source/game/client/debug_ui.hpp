#pragma once

// ========================================================================== //
// Headers
// ========================================================================== //

#include "core/types.hpp"
#include "core/macros.hpp"

// ========================================================================== //
// DebugUI Declaration
// ========================================================================== //

namespace dib::game {

DIB_FORWARD_DECLARE_CLASS(GameClient);

// -------------------------------------------------------------------------- //

/** Show statistics of the engine and the game **/
void
ShowStatisticsDebug(GameClient& gameClient, f32 delta);

// -------------------------------------------------------------------------- //

/** Show debugging information related to scripting **/
void
ShowScriptDebug(GameClient& gameClient);

// -------------------------------------------------------------------------- //

/** Show debugging information related to the loaded mods **/
void
ShowModDebug(GameClient& gameClient);

// -------------------------------------------------------------------------- //

/** Show debugging information related to tiles and the world **/
void
ShowTileDebug(GameClient& gameClient);

// -------------------------------------------------------------------------- //

/** Show debugging information related to walls and the world **/
void
ShowWallDebug(GameClient& gameClient);

// -------------------------------------------------------------------------- //

/** Show debugging information related to items **/
void
ShowItemDebug(GameClient& gameClient);

// -------------------------------------------------------------------------- //

/** Show debugging information about world **/
void
ShowWorldDebug(GameClient& gameClient);

// -------------------------------------------------------------------------- //

void
ShowNetworkDebug(GameClient& gameClient);

void
ShowPlayerDebug(GameClient& gameClient);
}
