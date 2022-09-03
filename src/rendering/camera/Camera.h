#pragma once

#include <util/Types.h>



namespace Camera {
    auto Init() -> void;
    auto Update() -> void;

    auto GetView() -> mat4;
    auto GetProjection() -> mat4;
    auto GetMatrix() -> mat4;

    auto GetPosition() -> vec3;
    auto GetTarget() -> vec3;

    auto AddAngleDelta(const vec2 angleDelta_) -> void;
    auto AddZoomDelta(const float zoomDelta_) -> void;

    auto SetTarget(const vec3 target_) -> void;
};