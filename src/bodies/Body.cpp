#include "Body.h"

#include <util/Constants.h>

using std::move;



Body::Body(string id, string name, bvec3 position, bvec3 velocity)
    : id(move(id)), name(move(name)), position(move(position)), velocity(move(velocity)) {}

auto Body::GetId() const -> string {
    return id;
}

auto Body::GetName() const -> string {
    return name;
}

auto Body::GetScaledPosition() const -> vec3 {
    return (position / SCALE_FACTOR).asVec3();
}

auto Body::GetScaledVelocity() const -> vec3 {
    return (velocity / SCALE_FACTOR).asVec3();
}