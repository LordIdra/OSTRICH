#pragma once

#include <util/Types.h>



namespace Mouse {
    auto SetCallbackLeftPress(void (*function)()) -> void;
    auto SetCallbackLeftRelease(void (*function)()) -> void;
    auto SetCallbackLeftDouble(void (*function)()) -> void;

    auto SetCallbackRightPress(void (*function)()) -> void;
    auto SetCallbackRightRelease(void (*function)()) -> void;

    auto LeftButtonHeld() -> bool;
    auto RightButtonHeld() -> bool;

    auto Show() -> void;
    auto Hide() -> void;
}