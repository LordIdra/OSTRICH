#include "Body.h"

#include <util/Constants.h>



Body::Body(bvec3 position, bvec3 velocity, string name)
    : position(std::move(position)), velocity(std::move(velocity)), name(std::move(name)) {}

auto Body::GetScaledPosition() const -> vec3 {
    return (position / SCALE_FACTOR).asVec3();
}

auto Body::GetScaledVelocity() const -> vec3 {
    return (velocity / SCALE_FACTOR).asVec3();
}

auto Body::GetName() const -> string {
    return name;
}