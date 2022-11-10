#pragma once

#include <util/Types.h>



namespace Mouse {
    auto AddCallbackLeftPress(void (*function)()) -> void;
    auto AddCallbackLeftRelease(void (*function)()) -> void;
    auto AddCallbackLeftDouble(void (*function)()) -> void;

    auto AddCallbackRightPress(void (*function)()) -> void;
    auto AddCallbackRightRelease(void (*function)()) -> void;

    auto LeftButtonHeld() -> bool;
    auto RightButtonHeld() -> bool;

    auto Show() -> void;
    auto Hide() -> void;
}