#pragma once

#include <bodies/Massive.h>
#include <util/Types.h>



namespace Rays {
    
    auto ScreenToWorld(const vec2 coords) -> vec3;
    auto WorldToScreen(const vec3 coords) -> vec2;
    auto IntersectsSphere(const vec3 cameraPosition, const vec3 cameraDirection, const vec3 spherePosition, const float sphereRadius) -> bool;
    auto RadiusOnScreen(const Massive massive) -> double;
}