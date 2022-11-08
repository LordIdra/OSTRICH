#pragma once

#include <bodies/Body.h>
#include <util/Types.h>



namespace Camera {
    auto Init() -> void;
    auto Reset() -> void;
    auto Update(const double deltaTime) -> void;

    auto StartTransition(const Body &body) -> void;
    auto UpdateTransitionTarget(const Body &body) -> void;

    auto GetView() -> mat4;
    auto GetProjection() -> mat4;
    auto GetMatrix() -> mat4;

    auto GetPosition() -> vec3;
    auto GetTarget() -> vec3;
    auto GetZoom() -> float;

    auto AddAngleDelta(const vec2 angleDelta_) -> void;
    auto AddZoomDelta(const float zoomDelta_) -> void;

    auto SetTarget(const vec3 target_) -> void;
    auto SetZoom(const float zoom_) -> void;
};