#pragma once

#include <bodies/Body.h>



class Massless : public Body {
public:
    Massless(uvec3 position, uvec3 velocity, string name);
};
