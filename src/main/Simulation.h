#pragma once

#include <bodies/Massive.h>
#include <unordered_map>



namespace Simulation {

    // Time steps per seconD
    const bfloat TIME_STEP_SIZE = 10000; 

    auto Distance(const bvec3 vec1, const bvec3 vec2) -> bfloat;

    auto Integrate(const unordered_map<string, Massive> &massive_bodies, Body &body) -> void;
}