#pragma once

#include <util/Types.h>



class Body {
protected:
    string id;
    string name;
    dvec3 position; // m
    dvec3 velocity; // m/s

public:
    Body(string id, string name, dvec3 position, dvec3 velocity);

    auto GetId() const -> string;
    auto GetName() const -> string;
    auto GetVelocity() const -> dvec3;
    auto GetPosition() const -> dvec3;
    
    auto GetScaledPosition() const -> vec3;
    auto GetScaledVelocity() const -> vec3;

    auto AddVelocity(dvec3 v) -> void;
    auto AddPosition(dvec3 x) -> void;
};
