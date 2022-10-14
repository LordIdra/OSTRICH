#include "Body.h"

#include <util/Constants.h>
#include <util/Log.h>



Body::Body(const string &id, const string &name, const vec3 &color, const double mass, const dvec3 &position, const dvec3 &velocity)
    : id(std::move(id)), name(std::move(name)), color(color), mass(mass), position(position), velocity(velocity) {}

auto Body::GetId() const -> string {
    return id;
}

auto Body::GetName() const -> string {
    return name;
}

auto Body::GetColor() const -> vec3 { 
    return color;
}

auto Body::GetMass() const -> double {
    return mass;
}

auto Body::GetVelocity() const -> dvec3 {
    return velocity;
}

auto Body::GetPosition() const -> dvec3 {
    return position;
}

auto Body::GetScaledPosition() const -> vec3 {
    return position / SCALE_FACTOR;
}

auto Body::GetScaledVelocity() const -> vec3 {
    return velocity / SCALE_FACTOR;
}

auto Body::AddVelocity(const dvec3 v) -> void {
    velocity += v;
}

auto Body::AddPosition(const dvec3 x) -> void {
    position += x;
}
