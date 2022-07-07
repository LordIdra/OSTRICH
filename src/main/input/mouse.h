#pragma once

#include "../window/window.h"



namespace mouse {
    auto Init() -> void;
    auto Update() -> void;

    auto GetPositionDelta() -> glm::vec2; // returns the change in mouse position since last frame (update call)
    auto GetScrollDelta() -> glm::vec2;   // returns the change in scroll position since last frame (update call)

    auto Show() -> void;
    auto Hide() -> void;
}