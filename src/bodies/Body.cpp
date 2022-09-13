#include "Body.h"

#include <util/Constants.h>
#include <util/Log.h>

using std::move;



Body::Body(string id, string name, bvec3 position, bvec3 velocity)
    : id(move(id)), name(move(name)), position(move(position)), velocity(move(velocity)) {}

auto Body::GetId() const -> string {
    return id;
}

auto Body::GetName() const -> string {
    return name;
}

auto Body::GetVelocity() const -> bvec3 {
    return velocity;
}

auto Body::GetPosition() const -> bvec3 {
    return position;
}

auto Body::GetScaledPosition() const -> vec3 {
    return (position / SCALE_FACTOR).AsVec3();
}

auto Body::GetScaledVelocity() const -> vec3 {
    return (velocity / SCALE_FACTOR).AsVec3();
}

auto Body::AddVelocity(const bvec3 v) -> void {
    velocity += v;
}

auto Body::AddPosition(const bvec3 x) -> void {
    position += x;
}
