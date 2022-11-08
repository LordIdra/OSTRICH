#pragma once

#include <bodies/Massive.h>



namespace MassiveRender {
    auto Init() -> void;
    auto Reset() -> void;
    auto Update(const double deltaTime) -> void;
    auto AddBody(const Massive &body) -> void;
}