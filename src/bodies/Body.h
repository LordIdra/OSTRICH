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
    auto GetVelocity() const -> bvec3;
    auto GetPosition() const -> bvec3;
    
    auto GetScaledPosition() const -> vec3;
    auto GetScaledVelocity() const -> vec3;

    auto AddVelocity(bvec3 v) -> void;
    auto AddPosition(bvec3 x) -> void;
};
