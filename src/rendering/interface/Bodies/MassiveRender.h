#pragma once

#include <bodies/Massive.h>



namespace MassiveRender {
    auto Init() -> void;
    auto Update(double deltaTime) -> void;
    auto AddBody(const Massive &body) -> void;
    auto StartTransition(const Body &body) -> void;
    auto UpdateTransitionTarget(const Body &body) -> void;
}