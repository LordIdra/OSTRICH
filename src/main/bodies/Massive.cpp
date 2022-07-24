#include "Massive.h"

#include "../rendering/geometry.h"

#include "../util/logging.h"



Massive::Massive(bvec3 position, bvec3 velocity, string name, bfloat mass, bfloat radius, vec3 color)
    : Body(position, velocity, name), mass(mass), radius(radius), color(color) {}

float Massive::ScaledRadius() {
    return float(radius / SCALE_FACTOR);
}

vector<VERTEX_DATA_TYPE> Massive::GetSphereVertices() {
    // steps is constant for now, but it may be wise to modify this in future
    // so that the number of steps depends on the radius of the sphere
    return geometry::Sphere(ScaledPosition(), ScaledRadius(), PI / 24);
}
