#pragma once

#include <bodies/Massive.h>
#include <util/Types.h>



namespace Bodies {
    auto Init() -> void;
    auto GetMassiveBody(const string &id) -> Massive;
}