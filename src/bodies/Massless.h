#pragma once

#include <bodies/Body.h>



class Massless : public Body {
public:
    Massless(string id, string name, dvec3 position, dvec3 velocity) : Body(id, name, position, velocity) {}
};
