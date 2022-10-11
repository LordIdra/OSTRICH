#include "MassiveIcon.h"

#include <rendering/geometry/Rays.h>



MassiveIcon::MassiveIcon(const Massive parent) 
    : parent(parent) {}

auto MassiveIcon::AddChild(const Massive body) -> void {
    massiveChildren.push_back(body);
}

auto MassiveIcon::AddChild(const Massless body) -> void {
    masslessChildren.push_back(body);
}

auto MassiveIcon::GetBody() const -> Massive {
    return parent;
}

auto MassiveIcon::GetColor() const -> vec3 {
    return parent.GetMaterial().diffuse;
}

auto MassiveIcon::GetNormalizedScreenCoordinates() const -> vec2 {
    return Rays::WorldToScreen(parent.GetScaledPosition());
}