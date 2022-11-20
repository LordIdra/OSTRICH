#pragma once

#include <rendering/structures/Material.h>
#include <util/Constants.h>
#include <bodies/Body.h>



class Massive : public Body {
private:
    Material material;
    vector<VERTEX_DATA_TYPE> vertices;

public:
    Massive(const string &id, const string &name, const vec3 &color, const dvec3 &position, const dvec3 &velocity, const Material &material, const double mass, const double radius);

    auto GetScaledRadius() const -> float;
    auto GetSphereVertices() const -> vector<VERTEX_DATA_TYPE>;
    auto GetMaterial() const -> Material;
    auto GetMatrix() const -> mat4;
    auto GetMinZoom() const -> double;
};
