#pragma once

#include <rendering/structures/Material.h>
#include <util/Constants.h>
#include <bodies/Body.h>



class Massive : public Body {
private:
    const float STEP = PI / 24;
    bfloat mass;   // kg
    bfloat radius; // m
    vector<VERTEX_DATA_TYPE> vertices;
    Material material;

public:
    Massive(const string &id, const string &name, const bvec3 &position, const bvec3 &velocity, const Material &material, const bfloat mass, const bfloat radius);

    auto GetMass() const -> bfloat;
    auto GetScaledRadius() const -> float;
    auto GetSphereVertices() const -> vector<VERTEX_DATA_TYPE>;
    auto GetMaterial() const -> Material;
    auto GetMatrix() const -> mat4;
};
