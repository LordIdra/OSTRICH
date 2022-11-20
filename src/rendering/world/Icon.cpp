#include "Icon.h"
#include "rendering/geometry/Rays.h"

#include <window/Window.h>
#include <input/Mouse.h>
#include <util/Log.h>
#include <main/Bodies.h>



const float Icon::ICON_MAIN_RADIUS = 10;
const float Icon::ICON_OUTLINE_RADIUS = 13;
const float Icon::ICON_HOVER_INNER_RADIUS = 19;
const float Icon::ICON_HOVER_OUTER_RADIUS = 24;
const float Icon::ICON_SELECTED_RADIUS = 24;

const vec3 Icon::ICON_BORDER_COLOR = vec3(1.0, 0.7, 0.1);
const vec3 Icon::ICON_HOVER_COLOR = vec3(1.0, 0.2, 0.1);
const vec3 Icon::ICON_SELECTED_COLOR = vec3(0.8, 0.2, 0.1);

const double Icon::RADIUS_THRESHOLD = 0.005;
const float Icon::MERGE_THRESHOLD = 24;
const float Icon::HOVER_THRESHOLD = 14;
const float Icon::SELECT_THRESHOLD = 20;



auto Icon::AddVertex(vector<float> &vertices, IconVertex vertex) -> void {
    // Vertex position
    vertices.push_back(vertex.x);
    vertices.push_back(vertex.y);

    // Colour
    vertices.push_back(vertex.r);
    vertices.push_back(vertex.g);
    vertices.push_back(vertex.b);
}

auto Icon::AddQuad(vector<float> &vertices, IconVertex v1, IconVertex v2, IconVertex v3, IconVertex v4) -> void {
    AddVertex(vertices, v1);
    AddVertex(vertices, v2);
    AddVertex(vertices, v3);

    AddVertex(vertices, v1);
    AddVertex(vertices, v3);
    AddVertex(vertices, v4);
}

auto Icon::AddIconCentre(vector<float> &vertices) const -> void {
    vec2 centre = GetNormalizedScreenCoordinates();
    float ICON_MAIN_RADIUS_X = ICON_MAIN_RADIUS / float(Window::GetWidth());
    float ICON_MAIN_RADIUS_Y = ICON_MAIN_RADIUS / float(Window::GetHeight());

    AddQuad(vertices, 
        IconVertex(centre.x, centre.y - ICON_MAIN_RADIUS_Y, GetColor()),
        IconVertex(centre.x - ICON_MAIN_RADIUS_X, centre.y, GetColor()),
        IconVertex(centre.x, centre.y + ICON_MAIN_RADIUS_Y, GetColor()),
        IconVertex(centre.x + ICON_MAIN_RADIUS_X, centre.y, GetColor()));
}

auto Icon::AddIconBorder(vector<float> &vertices) const -> void {
    vec2 centre = GetNormalizedScreenCoordinates();
    float ICON_OUTLINE_RADIUS_X = ICON_OUTLINE_RADIUS / float(Window::GetWidth());
    float ICON_OUTLINE_RADIUS_Y = ICON_OUTLINE_RADIUS / float(Window::GetHeight());

    AddQuad(vertices, 
        IconVertex(centre.x, centre.y - ICON_OUTLINE_RADIUS_Y, ICON_BORDER_COLOR),
        IconVertex(centre.x - ICON_OUTLINE_RADIUS_X, centre.y, ICON_BORDER_COLOR),
        IconVertex(centre.x, centre.y + ICON_OUTLINE_RADIUS_Y, ICON_BORDER_COLOR),
        IconVertex(centre.x + ICON_OUTLINE_RADIUS_X, centre.y, ICON_BORDER_COLOR));
}

auto Icon::AddIconSelected(vector<float> &vertices) const -> void {
    vec2 centre = GetNormalizedScreenCoordinates();
    float ICON_SELECTED_RADIUS_X = ICON_SELECTED_RADIUS / float(Window::GetWidth());
    float ICON_SELECTED_RADIUS_Y = ICON_SELECTED_RADIUS / float(Window::GetHeight());

    AddQuad(vertices, 
        IconVertex(centre.x, centre.y - ICON_SELECTED_RADIUS_Y, ICON_SELECTED_COLOR),
        IconVertex(centre.x - ICON_SELECTED_RADIUS_X, centre.y, ICON_SELECTED_COLOR),
        IconVertex(centre.x, centre.y + ICON_SELECTED_RADIUS_Y, ICON_SELECTED_COLOR),
        IconVertex(centre.x + ICON_SELECTED_RADIUS_X, centre.y, ICON_SELECTED_COLOR));
}

auto Icon::AddIconHover(vector<float> &vertices) const -> void {
    vec2 centre = GetNormalizedScreenCoordinates();
    float ICON_HOVER_INNER_RADIUS_X = ICON_HOVER_INNER_RADIUS / float(Window::GetWidth());
    float ICON_HOVER_INNER_RADIUS_Y = ICON_HOVER_INNER_RADIUS / float(Window::GetHeight());
    float ICON_HOVER_OUTER_RADIUS_X = ICON_HOVER_OUTER_RADIUS / float(Window::GetWidth());
    float ICON_HOVER_OUTER_RADIUS_Y = ICON_HOVER_OUTER_RADIUS / float(Window::GetHeight());

    AddQuad(vertices,
        IconVertex(centre.x, centre.y - ICON_HOVER_INNER_RADIUS_Y, ICON_HOVER_COLOR),
        IconVertex(centre.x - ICON_HOVER_INNER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
        IconVertex(centre.x - ICON_HOVER_OUTER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
        IconVertex(centre.x, centre.y - ICON_HOVER_OUTER_RADIUS_Y, ICON_HOVER_COLOR));

    AddQuad(vertices,
        IconVertex(centre.x, centre.y - ICON_HOVER_INNER_RADIUS_Y, ICON_HOVER_COLOR),
        IconVertex(centre.x + ICON_HOVER_INNER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
        IconVertex(centre.x + ICON_HOVER_OUTER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
        IconVertex(centre.x, centre.y - ICON_HOVER_OUTER_RADIUS_Y, ICON_HOVER_COLOR));

    AddQuad(vertices,
        IconVertex(centre.x, centre.y + ICON_HOVER_INNER_RADIUS_Y, ICON_HOVER_COLOR),
        IconVertex(centre.x - ICON_HOVER_INNER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
        IconVertex(centre.x - ICON_HOVER_OUTER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
        IconVertex(centre.x, centre.y + ICON_HOVER_OUTER_RADIUS_Y, ICON_HOVER_COLOR));

    AddQuad(vertices,
        IconVertex(centre.x, centre.y + ICON_HOVER_INNER_RADIUS_Y, ICON_HOVER_COLOR),
        IconVertex(centre.x + ICON_HOVER_INNER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
        IconVertex(centre.x + ICON_HOVER_OUTER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
        IconVertex(centre.x, centre.y + ICON_HOVER_OUTER_RADIUS_Y, ICON_HOVER_COLOR));
}

Icon::Icon(const Body &body) : body(body) {}

auto Icon::GetScreenCoordinates() const -> vec2 {
    return Rays::UnNormalize(GetNormalizedScreenCoordinates());
}

auto Icon::GetNormalizedScreenCoordinates() const -> vec2 {
    return Rays::WorldToScreen(body.GetScaledPosition());
}

auto Icon::AddChild(const string &id) -> void {
    children.push_back(id);
}

auto Icon::AddVertices(vector<float> &vertices) const -> void {
    // Centre + borders
    AddIconCentre(vertices);
    AddIconBorder(vertices);

    // Selected
    if (Bodies::GetSelectedBodyId() == GetId()) {
        AddIconSelected(vertices);
    }

    // Hover
    if (MouseOnIcon(HOVER_THRESHOLD)) {
        AddIconHover(vertices);
    }
}

auto Icon::MouseOnIcon(const float threshold) const -> bool {
    // Mouse coordinates are currently between 0 and 1, let's normalize that so they are 0 to width and 0 to height
    vec2 c = Mouse::GetUnNormalizedPosition();

    const vec2 k = GetScreenCoordinates();

    bool conditionY = std::abs(c.y - k.y) <  threshold;
    bool conditionX = std::abs(c.x - k.x) < (threshold - std::abs(c.y -  k.y));

    return conditionX && conditionY;
}

auto Icon::GetId() const -> string {
    return body.GetId();
}

auto Icon::GetColor() const -> vec3 {
    return body.GetColor();
}

auto Icon::GetMass() const -> double {
    return body.GetMass();
}