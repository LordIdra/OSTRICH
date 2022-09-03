#pragma once

#include <util/Types.h>



class Body {
protected:
    string id;
    string name;
    bvec3 position; // m
    bvec3 velocity; // m/s

public:
    Body(string id, string name, bvec3 position, bvec3 velocity);

    auto GetId() const -> string;
    auto GetName() const -> string;
    auto GetScaledPosition() const -> vec3;
    auto GetScaledVelocity() const -> vec3;
};
