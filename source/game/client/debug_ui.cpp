#include "game/client/debug_ui.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

#include "app/client/imgui/imgui.h"
#include "game/client/game_client.hpp"
#include "game/client/player_data_storage.hpp"
#include "script/util.hpp"

// ========================================================================== //
// DebugUI Implementation
// ========================================================================== //

namespace dib::game {

void
ShowScriptDebug(GameClient& gameClient)
{
  if (ImGui::CollapsingHeader("Script")) {
    static constexpr u32 bufferSize = 4096;
    static char inputBuffer[bufferSize];
    static char outputBuffer[bufferSize];
    if (ImGui::InputText("Script input",
                         inputBuffer,
                         bufferSize,
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
      JsValueRef output;
      script::Result result =
        gameClient.GetScriptEnvironment().RunScript(inputBuffer, output);
      if (result == script::Result::kException) {
        String exception = script::GetAndClearException();
        if (exception.GetSize() < 512) {
          inputBuffer[0] = 0;
          memcpy(outputBuffer, exception.GetUTF8(), exception.GetSize());
          outputBuffer[exception.GetSize()] = 0;
        }
      } else {
        String outputString = script::ToString(output);
        if (outputString.GetSize() < 512) {
          inputBuffer[0] = 0;
          memcpy(outputBuffer, outputString.GetUTF8(), outputString.GetSize());
          outputBuffer[outputString.GetSize()] = 0;
        }
        ImGui::SetKeyboardFocusHere(-1);
      }
    }

    ImGui::InputTextMultiline("Script output",
                              outputBuffer,
                              bufferSize,
                              ImVec2(),
                              ImGuiInputTextFlags_ReadOnly);
  }
}

// -------------------------------------------------------------------------- //

void
ShowModDebug(GameClient& gameClient)
{
  if (ImGui::CollapsingHeader("Mods")) {
    static s32 currentIndex = 0;
    std::vector<const char8*> names;
    for (auto& mod : gameClient.GetModLoader().GetMods()) {
      names.push_back(mod.first.GetUTF8());
    }
    ImGui::ListBox("", &currentIndex, names.data(), names.size());
    auto mod = gameClient.GetModLoader().GetModById(names[currentIndex]);
    ImGui::Text("Name: '%s'", mod->GetName().GetUTF8());
    ImGui::Text("Id: '%s'", mod->GetId().GetUTF8());
    ImGui::Text("Authors:");
    for (auto& author : mod->GetAuthors()) {
      ImGui::Text("  %s", author.GetUTF8());
    }
  }
}

// -------------------------------------------------------------------------- //

void
ShowTileDebug(GameClient& gameClient)
{
  TileManager& tileManager = gameClient.GetWorld().GetTileManager();
  Terrain& terrain = gameClient.GetWorld().GetTerrain();

  if (ImGui::CollapsingHeader("Tile")) {
    static s32 currentIndex = 0;
    std::vector<const char8*> names;
    for (auto& tile : tileManager.GetTiles()) {
      names.push_back(tile.id.GetUTF8());
    }
    ImGui::ListBox("", &currentIndex, names.data(), names.size());

    auto tile = tileManager.GetTile(names[currentIndex]);
    ImGui::Text("Id: %i", currentIndex);
    ImGui::Text("Resources: {%u}", u32(tile->GetResourcePaths().size()));
    for (auto& resource : tile->GetResourcePaths()) {
      ImGui::Text("  %s", resource.GetPath().GetPathString().GetUTF8());
    }

    ImGui::Separator();

    static s32 pos[2] = { 0, 0 };
    ImGui::InputInt("X", &pos[0], 1, 100);
    ImGui::InputInt("Y", &pos[1], 1, 100);
    pos[0] = alflib::Clamp(u32(pos[0]), 0u, terrain.GetWidth());
    pos[1] = alflib::Clamp(u32(pos[1]), 0u, terrain.GetHeight());
    std::shared_ptr<game::Tile> tileAtPos =
      terrain.GetTile(pos[0], pos[1], game::Terrain::LAYER_FOREGROUND);

    String tileAtPosStr =
      String("Tile at pos: {}, id {}")
        .Format(tileManager.GetTileID(tileAtPos->GetID()), tileAtPos->GetID());
    ImGui::Text("%s", tileAtPosStr.GetUTF8());

    Vector2F texMin, texMax;
    const game::ResourcePath& res =
      tileAtPos->GetResourcePath(gameClient.GetWorld(), pos[0], pos[1]);
    gameClient.GetTileAtlas().GetTextureCoordinates(res, texMin, texMax);

    ImGui::Image(reinterpret_cast<ImTextureID>(
                   gameClient.GetTileAtlas().GetTexture()->GetID()),
                 ImVec2(64, 64),
                 ImVec2(texMin.x, texMin.y),
                 ImVec2(texMax.x, texMax.y),
                 ImVec4(1, 1, 1, 1),
                 ImVec4(0, 0, 0, 1));

    // Change block
    if (gameClient.IsMouseDown(MouseButton::kMouseButtonLeft)) {
      f64 mouseX, mouseY;
      gameClient.GetMousePosition(mouseX, mouseY);
      u32 tileX, tileY;
      terrain.PickTile(gameClient.GetCamera(), mouseX, mouseY, tileX, tileY);
      terrain.SetTile(tile, tileX, tileY, game::Terrain::LAYER_FOREGROUND);
    }
  }
}

// -------------------------------------------------------------------------- //

void
ShowNetworkDebug(GameClient& gameClient)
{
  World& world = gameClient.GetWorld();
  auto& chat = world.GetChat();

  // ImGui::ShowTestWindow();

  if (ImGui::CollapsingHeader("Network")) {

    ImGui::Text("Network status: %s",
                world.GetNetwork().GetConnectionState() ==
                    ConnectionState::kConnected
                  ? "connected"
                  : "disconnected");

    if (world.GetNetwork().GetConnectionState() ==
        ConnectionState::kConnected) {
      if (ImGui::Button("disconnect")) {
        world.GetNetwork().Disconnect();
      }
    } else {
      constexpr std::size_t addrlen = 50;
      static char8 addr[addrlen] = "127.0.0.1:24812";
      ImGui::InputText("IP Adress", addr, addrlen);
      if (ImGui::Button("connect")) {
        world.GetNetwork().ConnectToServer(addr);
      }
    }

    if (ImGui::TreeNode("Chat")) {
      constexpr std::size_t buflen = 50;
      static char8 buf[buflen] = "Rully";
      ImGui::InputText("Name", buf, buflen);
      if (ImGui::Button("Set Name")) {
        if (world.GetNetwork().GetConnectionState() ==
            ConnectionState::kDisconnected) {
          auto& player_data = PlayerDataStorage::Load();
          player_data.name = buf;
        }
      }
      ImGui::Text("Note: name can only be applied when disconnected.");

      ImGui::Spacing();
      ImGui::Spacing();

      constexpr std::size_t textlen = 128;
      static char8 text[textlen] = "";
      ImGui::InputText("chat", text, textlen);
      if (ImGui::Button("send chat message")) {
        game::ChatMessage msg{};
        msg.msg = String(text);
        msg.type = game::ChatType::kSay;
        msg.uuid_from = world.GetNetwork().GetOurUuid();
        if (!chat.SendMessage(msg)) {
          DLOG_WARNING("failed to send chat message");
        }
        text[0] = '\0';
      }

      ImGui::InputTextMultiline("",
                                const_cast<char8*>(chat.GetDebug().GetUTF8()),
                                chat.GetDebug().GetSize(),
                                ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
                                ImGuiInputTextFlags_AllowTabInput |
                                  ImGuiInputTextFlags_ReadOnly);

      ImGui::TreePop();
    }
  }
}

}