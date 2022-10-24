#pragma once

#include <bodies/Massive.h>
#include <util/Types.h>



namespace Rays {
    
    auto Scale(const dvec3 coordinate) -> vec3;
    auto Unscale(const vec3 coordinate) -> dvec3;
    
    auto ScreenToWorld(const vec2 coords) -> vec3;
    auto WorldToScreen(const vec3 coords) -> vec2;

    auto UnNormalize(const vec2 coords) -> vec2;

    auto IntersectsSphere(const vec3 cameraPosition, const vec3 cameraDirection, const vec3 spherePosition, const float sphereRadius) -> bool;
    auto RadiusOnScreen(const Massive &massive) -> double;

    auto IsCoordinateOffCamera(const vec3 &coordinate) -> bool;
    auto IsCoordinateOffCamera(const vec2 &coordinate) -> bool;
}