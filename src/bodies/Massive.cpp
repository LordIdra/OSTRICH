#include "Massive.h"

#include <util/Log.h>
#include <rendering/Geometry.h>

using std::move;


Massive::Massive(const string &id, const string &name, const bvec3 &position, const bvec3 &velocity, bfloat mass, bfloat radius)
    : Body(id, name, position, velocity), mass(move(mass)), radius(move(radius)) {}

auto Massive::GetScaledRadius() const -> float {
    return float(radius / SCALE_FACTOR);
}

auto Massive::GetSphereVertices() const -> vector<VERTEX_DATA_TYPE>{
    return Geometry::Sphere(GetScaledPosition(), GetScaledRadius(), STEP);
}
