#pragma once

#include "Types.h"



namespace Rays {
    
    auto ScreenToWorld(vec2 coords) -> vec3;

    auto IntersectsSphere(vec3 cameraPosition, vec3 cameraDirection, vec3 spherePosition, float sphereRadius) -> bool;
}