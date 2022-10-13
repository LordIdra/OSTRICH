#include "MassiveIcon.h"
#include "window/Window.h"

#include <rendering/geometry/Rays.h>



MassiveIcon::MassiveIcon(const Massive &parent) 
    : Icon(parent.GetId(), parent.GetMaterial().diffuse), parent(parent) {}

auto MassiveIcon::AddChild(const Massive &body) -> void {
    massiveChildren.push_back(body);
}

auto MassiveIcon::AddChild(const Massless &body) -> void {
    masslessChildren.push_back(body);
}

auto MassiveIcon::GetBody() const -> Massive {
    return parent;
}

auto MassiveIcon::GetScreenCoordinates() const -> vec2 {
    return Rays::UnNormalize(GetNormalizedScreenCoordinates());
}

auto MassiveIcon::GetNormalizedScreenCoordinates() const -> vec2 {
    return Rays::WorldToScreen(parent.GetScaledPosition());
}