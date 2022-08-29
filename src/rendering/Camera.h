#include "../util/Types.h"



namespace Camera {
    // a typical camera system uses two matrices, that are multiplied together to form a final camera matrix
    // this final matrix is used to transform the points of every object on the scene
    // GetMatrix() will return said final matrix
    auto GetView() -> mat4;
    auto GetProjection() -> mat4;
    auto GetMatrix() -> mat4;

    auto GetPosition() -> vec3;
    auto GetTarget() -> vec3;

    auto AddAngleDelta(const vec2 angleDelta_) -> void;
    auto AddZoomDelta(const float zoomDelta_) -> void;

    auto SetTarget(const vec3 target_) -> void;

    auto Update() -> void;
};