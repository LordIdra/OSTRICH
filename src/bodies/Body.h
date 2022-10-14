#pragma once

#include <util/Types.h>



class Body {
protected:
    string id;
    string name;
    vec3 color;
    double mass;
    dvec3 position; // m
    dvec3 velocity; // m/s

public:
    Body(const string &id, const string &name, const vec3 &color, const double mass, const dvec3 &position, const dvec3 &velocity);

    auto GetId() const -> string;
    auto GetName() const -> string;
    auto GetColor() const -> vec3;
    auto GetMass() const -> double;
    auto GetVelocity() const -> dvec3;
    auto GetPosition() const -> dvec3;
    
    auto GetScaledPosition() const -> vec3;
    auto GetScaledVelocity() const -> vec3;

    auto AddVelocity(dvec3 v) -> void;
    auto AddPosition(dvec3 x) -> void;
};
