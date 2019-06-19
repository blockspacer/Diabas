#pragma once

#include "app/app.hpp"
#include "game/world.hpp"
#include "server/input.hpp"
#include "network/network.hpp"
#include "script/env.hpp"

// ========================================================================== //
// Game Declaration
// ========================================================================== //

namespace dib {

/** Game class**/
class Game : public Application
{

  // ============================================================ //
  // Lifetime
  // ============================================================ //
public:
  explicit Game(const Descriptor& descriptor);

  ~Game() override;

  // ============================================================ //
  // Methods
  // ============================================================ //
public:
  void Update(f64 delta) override;

  void Render() override;

  // ============================================================ //
  // Constants
  // ============================================================ //
public:
#ifdef DIB_IS_SERVER
  static constexpr Side kSide = Side::kServer;
#else
  static constexpr Side kSide = Side::kClient;
#endif

  // ============================================================ //
  // Private Methods
  // ============================================================ //
private:
  void SetupInputCommands();

  // ============================================================ //
  // Member Variables
  // ============================================================ //
private:
  /** Script environment **/
  script::Environment mScriptEnvironment;

  // MUST be before any other network code.
  Network network_{};

  World<kSide> world_{};

  InputHandler<kSide> input_handler_{};
};

}