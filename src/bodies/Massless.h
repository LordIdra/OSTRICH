#pragma once

#include <bodies/Body.h>



class Massless : public Body {
public:
    Massless(const string &id, const string &name, const vec3 &color, const dvec3 &position, const dvec3 &velocity, const double mass, const double radius) 
        : Body(id, name, color, mass, radius, position, velocity) {}
};
