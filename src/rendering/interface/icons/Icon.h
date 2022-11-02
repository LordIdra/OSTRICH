#pragma once

#include <bodies/Massive.h>
#include <bodies/Massless.h>
#include <util/Types.h>
#include <rendering/interface/icons/IconVertex.h>



class Icon {
private:
    static const float ICON_MAIN_RADIUS;
    static const float ICON_OUTLINE_RADIUS;
    static const float ICON_HOVER_INNER_RADIUS;
    static const float ICON_HOVER_OUTER_RADIUS;
    static const float ICON_SELECTED_RADIUS;

    static const vec3 ICON_BORDER_COLOR;
    static const vec3 ICON_HOVER_COLOR;
    static const vec3 ICON_SELECTED_COLOR;

    Body body;

    vector<string> children;

    static auto AddVertex(vector<float> &vertices, IconVertex vertex) -> void;
    static auto AddQuad(vector<float> &vertices, IconVertex v1, IconVertex v2, IconVertex v3, IconVertex v4) -> void;

    auto AddIconCentre(vector<float> &vertices) const -> void;
    auto AddIconBorder(vector<float> &vertices) const -> void;
    auto AddIconSelected(vector<float> &vertices) const -> void;
    auto AddIconHover(vector<float> &vertices) const -> void;


public:
    static const double RADIUS_THRESHOLD;
    static const float MERGE_THRESHOLD;
    static const float HOVER_THRESHOLD;
    static const float SELECT_THRESHOLD;

    Icon(const Body &body);

    auto GetScreenCoordinates() const -> vec2;
    auto GetNormalizedScreenCoordinates() const -> vec2;

    auto AddChild(const string &id) -> void;

    auto AddVertices(vector<float> &vertices) const -> void;
    auto MouseOnIcon(const float threshold) const -> bool;

    auto GetId() const -> string;
    auto GetColor() const -> vec3;
    auto GetMass() const -> double;
};