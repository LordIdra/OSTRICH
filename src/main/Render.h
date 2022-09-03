#pragma once

#include <bodies/Massive.h>



namespace Render {
    auto Init() -> void;
    auto Update(double deltaTime) -> void;
    auto AddBody(const Massive &body) -> void;
    auto StartTransition(const Massive &body) -> void;
}