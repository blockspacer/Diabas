#pragma once

// ========================================================================== //
// Headers
// ========================================================================== //

#include "core/types.hpp"

// ========================================================================== //
// Functions
// ========================================================================== //

namespace dib::core {

void
GetVirtualMemoryUsage(f64& vmUsage, f64& residentSet);

}