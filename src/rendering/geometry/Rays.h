#pragma once

#include <util/Types.h>



namespace Rays {
    
    auto ScreenToWorld(const vec2 coords) -> vec3;
    auto IntersectsSphere(const vec3 cameraPosition, const vec3 cameraDirection, const vec3 spherePosition, const float sphereRadius) -> bool;
}