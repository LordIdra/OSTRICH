#pragma once

#include <bodies/Massive.h>
#include <util/Types.h>



namespace Bodies {
    auto Init() -> void;
    auto Update() -> void;
    auto GetMassiveBodies() -> unordered_map<string, Massive>;
    auto GetMassiveBody(const string &id) -> Massive;
}