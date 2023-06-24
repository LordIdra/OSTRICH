#pragma once

#include <util/Types.h>



class Body {
protected:
    const float MASSLESS_MIN_ZOOM = 0.0001;
    string id;
    string name;
    vec3 color;
    double mass;
    double radius;
    dvec3 position; // m
    dvec3 velocity; // m/s

public:
    Body(const string &id, const string &name, const vec3 &color, const double mass, const double radius, const dvec3 &position, const dvec3 &velocity);

    auto GetId() const -> string;
    auto GetName() const -> string;
    auto GetColor() const -> vec3;
    auto GetMass() const -> double;
    auto GetRadius() const -> double;
    auto GetVelocity() const -> dvec3;
    auto GetPosition() const -> dvec3;
    
    auto GetScaledPosition() const -> vec3;
    auto GetScaledVelocity() const -> vec3;

    auto SetVelocity(const dvec3 v) -> void;

    auto SetPosition(const dvec3 x) -> void;

    auto AddVelocity(dvec3 v) -> void;
    auto AddPosition(dvec3 x) -> void;

    virtual auto GetMinZoom() const -> double;
};
