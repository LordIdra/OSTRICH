#pragma once

#include <util/Types.h>



namespace Mouse {
    auto GetNormalizedPosition() -> vec2;
    auto GetScreenPosition() -> vec2;
    auto GetUnNormalizedPosition() -> vec2;
    auto GetPositionDelta() -> vec2;
}