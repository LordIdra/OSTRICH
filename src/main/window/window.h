#pragma once

#include "../util/Types.h"



namespace window {
    auto Init(const string &title) -> void;
    auto Update() -> void;

    auto Background(vec4 color) -> void;

    auto GetWidth() -> int;
    auto GetHeight() -> int;

    auto ShowCursor() -> void;
    auto HideCursor() -> void;

    auto Close() -> void;
    auto ShouldClose() -> bool;
};
