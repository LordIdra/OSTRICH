#include "Body.h"

#include "../util/Constants.h"



Body::Body(bvec3 position, bvec3 velocity, string name) 
    : position(position), velocity(velocity), name(name) {}

vec3 Body::ScaledPosition() {
    return (position / SCALE_FACTOR).asVec3();
}

vec3 Body::ScaledVelocity() {
    return (velocity / SCALE_FACTOR).asVec3();
}
