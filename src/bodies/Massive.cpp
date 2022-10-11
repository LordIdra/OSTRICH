#include "Massive.h"

#include <glm/ext/matrix_transform.hpp>
#include <util/Log.h>
#include <rendering/geometry/Sphere.h>

using std::move;


Massive::Massive(const string &id, const string &name, const dvec3 &position, const dvec3 &velocity, const Material &material, const double mass, const double radius)
    : Body(id, name, position, velocity), material(material), mass(move(mass)), radius(move(radius)) {
        vertices = Sphere::Sphere(GetScaledRadius(), SPHERE_STEP);
}

auto Massive::GetMass() const -> double {
    return mass;
}

auto Massive::GetScaledRadius() const -> float {
    return float(radius / SCALE_FACTOR);
}

auto Massive::GetSphereVertices() const -> vector<VERTEX_DATA_TYPE> {
    return vertices;
}

auto Massive::GetMaterial() const -> Material {
    return material;
}

auto Massive::GetMatrix() const -> mat4 {
    return glm::translate(mat4(1.0), GetScaledPosition());
}
