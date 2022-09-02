#include "Massive.h"

#include <util/Log.h>
#include <rendering/Geometry.h>



Massive::Massive(const bvec3 &position, const bvec3 &velocity, const string &name, bfloat mass, bfloat radius)
    : Body(position, velocity, name), mass(std::move(mass)), radius(std::move(radius)) {}

auto Massive::GetScaledRadius() const -> float {
    return float(radius / SCALE_FACTOR);
}

auto Massive::GetSphereVertices() const -> vector<VERTEX_DATA_TYPE>{
    return Geometry::Sphere(GetScaledPosition(), GetScaledRadius(), STEP);
}
