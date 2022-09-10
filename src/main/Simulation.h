#pragma once

#include <bodies/Massive.h>



namespace Simulation {
    const float TIME_STEP_SIZE = 1000;

    auto Distance(const bvec3 vec1, const bvec3 vec2) -> bfloat;

    auto Integrate(vector<Massive> &massive_bodies) -> void;
}