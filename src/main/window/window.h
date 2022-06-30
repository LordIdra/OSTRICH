#include "../util/Types.h"



namespace window {
    auto Init(const string &title) -> void;

    auto GetWidth() -> int;
    auto GetHeight() -> int;

    auto ShowCursor() -> void;
    auto HideCursor() -> void;
};

