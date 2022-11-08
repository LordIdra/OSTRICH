#pragma once

#include <util/Types.h>


namespace CameraTransition {
    auto Init() -> void;
    auto Reset() -> void;
    auto Update(const double deltaTime) -> void;
    auto SetTargetBody(const string &id) -> void;
}