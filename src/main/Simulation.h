#pragma once

#include <bodies/Massive.h>
#include <unordered_map>



namespace Simulation {

    const bfloat TIME_STEPS_PER_FRAME = 100; 
    const bfloat TIME_STEP_SIZE = 1000;

    auto Distance(const bvec3 vec1, const bvec3 vec2) -> bfloat;

    auto Integrate(const unordered_map<string, Massive> &massive_bodies, Body &body) -> void;
    auto Integrate(unordered_map<string, Massive> &massive_bodies) -> void;
}