#pragma once

#include <util/Types.h>
#include <rendering/world/Icon.h>



namespace Icons {
    auto Init() -> void;
    auto GenerateIcons() -> unordered_map<string, Icon>;
    auto Update() -> void;
}