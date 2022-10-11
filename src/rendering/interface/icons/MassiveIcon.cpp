#include "MassiveIcon.h"
#include "window/Window.h"

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

auto MassiveIcon::GetScreenCoordinates() const -> vec2 {
    return Rays::UnNormalize(GetNormalizedScreenCoordinates());
    //return vec2(0.5, 0.5) * (GetNormalizedScreenCoordinates() + vec2(1, 1)) * vec2(Window::GetWidth(), Window::GetHeight());
}

auto MassiveIcon::GetNormalizedScreenCoordinates() const -> vec2 {
    return Rays::WorldToScreen(parent.GetScaledPosition());
}