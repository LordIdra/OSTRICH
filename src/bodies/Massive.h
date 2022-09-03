#pragma once

#include <util/Constants.h>
#include <bodies/Body.h>



class Massive : public Body {
private:
    const float STEP = PI / 24;
    bfloat mass;   // kg
    bfloat radius; // m

public:
    Massive(const string &id, const string &name, const bvec3 &position, const bvec3 &velocity, const bfloat mass, const bfloat radius);

    auto GetScaledRadius() const -> float;
    auto GetSphereVertices() const -> vector<VERTEX_DATA_TYPE>;
};
