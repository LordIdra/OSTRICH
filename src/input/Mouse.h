#pragma once

#include <util/Types.h>



namespace Mouse {
    auto Init() -> void;
    auto Update() -> void;

    auto LeftButtonHeld() -> bool;
    auto RightButtonHeld() -> bool;

    auto SetCallbackLeftPress(void (*function)()) -> void;
    auto SetCallbackLeftRelease(void (*function)()) -> void;
    auto SetCallbackLeftDouble(void (*function)()) -> void;

    auto SetCallbackRightPress(void (*function)()) -> void;
    auto SetCallbackRightRelease(void (*function)()) -> void;

    auto GetPosition() -> vec2;
    auto GetPositionDelta() -> vec2; // returns the change in mouse position since last frame (update call)
    auto GetScrollDelta() -> vec2;   // returns the change in scroll position since last frame (update call)

    auto Show() -> void;
    auto Hide() -> void;
}