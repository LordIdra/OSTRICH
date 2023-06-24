#pragma once

#include <bodies/BodyType.h>
#include <simulation/OrbitPoint.h>
#include <bodies/Massless.h>
#include <bodies/Massive.h>
#include <util/Types.h>



namespace Bodies {
    auto PreReset() -> void;
    auto PostReset() -> void;

    auto AddBody(const Massive &body) -> void;
    auto AddBody(const Massless &body) -> void;

    auto UpdateBody(const string &id, const OrbitPoint &point) -> void;

    auto SetSelectedBody(const string &id) -> void;
    auto GetSelectedBody() -> const Body&;
    auto GetSelectedBodyId() -> string;
    auto IsBodySelected() -> bool;

    auto GetMinZoom() -> float;

    auto GetMassiveBodies() -> const unordered_map<string, Massive>&;
    auto GetMasslessBodies() -> const unordered_map<string, Massless>&;
    auto GetBodies() -> const unordered_map<string, Body>&;

    auto GetBodyIds() -> const vector<string>&;

    auto GetMassiveBody(const string &id) -> const Massive&;
    auto GetMasslessBody(const string &id) -> const Massless&;
    auto GetBody(const string &id) -> const Body&;

    auto GetBodyCount() -> unsigned int;
}