#pragma once

#include <bodies/Massive.h>
#include <bodies/Massless.h>



class MassiveIcon {
private:
    Massive parent;
    vector<Massive> massiveChildren;
    vector<Massless> masslessChildren;


public:
    MassiveIcon(const Massive &parent);

    auto AddChild(const Massive &body) -> void;
    auto AddChild(const Massless &body) -> void;

    auto GetBody() const -> Massive;
    auto GetColor() const -> vec3;
    
    auto GetScreenCoordinates() const-> vec2;
    auto GetNormalizedScreenCoordinates() const-> vec2;
};