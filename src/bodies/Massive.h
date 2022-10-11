#pragma once

#include <rendering/structures/Material.h>
#include <util/Constants.h>
#include <bodies/Body.h>



class Massive : public Body {
private:
    double mass;   // kg
    double radius; // m
    vector<VERTEX_DATA_TYPE> vertices;
    Material material;

public:
    Massive(const string &id, const string &name, const dvec3 &position, const dvec3 &velocity, const Material &material, const double mass, const double radius);

    auto GetMass() const -> double;
    auto GetScaledRadius() const -> float;
    auto GetSphereVertices() const -> vector<VERTEX_DATA_TYPE>;
    auto GetMaterial() const -> Material;
    auto GetMatrix() const -> mat4;
};
