#pragma once

#include <bodies/Massive.h>



namespace MassiveRender {
    auto Init() -> void;
    auto PreReset() -> void;
    auto Update() -> void;
    auto AddBody(const Massive &body) -> void;
}