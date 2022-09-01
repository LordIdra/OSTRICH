#include "Body.h"

#include <util/Constants.h>



Body::Body(bvec3 position, bvec3 velocity, string name) 
    : position(position), velocity(velocity), name(name) {}

auto Body::ScaledPosition() -> vec3 {
    return (position / SCALE_FACTOR).asVec3();
}

auto Body::ScaledVelocity() -> vec3 {
    return (velocity / SCALE_FACTOR).asVec3();
}

auto Body::GetName() -> string {
    return name;
}