#pragma once

#include "mouse/Buttons.h"
#include "mouse/Position.h"
#include "mouse/Scroll.h"



namespace Mouse {
    auto Init() -> void;
    auto Update() -> void;
}