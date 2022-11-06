#pragma once

#include <main/OrbitPoint.h>
#include <bodies/BodyType.h>
#include <bodies/Massless.h>
#include <bodies/Massive.h>
#include <util/Types.h>



namespace Bodies {
    auto Init() -> void;
    auto Update(const double deltaTime) -> void;

    auto AddBody(const Massive &body) -> void;
    auto AddBody(const Massless &body) -> void;

    auto SetSelectedBody(const string &id) -> void;
    auto GetSelectedBody() -> string;
    auto GetSelectedType() -> BodyType;

    auto GetMinZoom() -> float;

    auto GetMassiveBodies() -> unordered_map<string, Massive>;
    auto GetMasslessBodies() -> unordered_map<string, Massless>;

    auto GetMassiveBody(const string &id) -> Massive;
    auto GetMasslessBody(const string &id) -> Massless;
    auto GetBody(const string &id) -> Body;

    auto GetPastPoints() -> unordered_map<string, vector<OrbitPoint>>;
    auto GetFuturePoints() -> unordered_map<string, vector<OrbitPoint>>;
}