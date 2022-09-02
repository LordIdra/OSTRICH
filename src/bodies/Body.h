#pragma once

#include <util/Types.h>



class Body {
protected:
    bvec3 position; // m
    bvec3 velocity; // m/s
    string name;

public:
    Body(bvec3 position, bvec3 velocity, string name);

    auto GetName() const -> string;
    auto GetScaledPosition() const -> vec3;
    auto GetScaledVelocity() const -> vec3;
    auto GetIconVertices() const -> vector<VERTEX_DATA_TYPE>;
};
