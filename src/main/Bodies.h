#pragma once

#include <bodies/Massless.h>
#include <bodies/Massive.h>
#include <util/Types.h>



namespace Bodies {
    auto Init() -> void;
    auto Update() -> void;

    auto SetSelectedBody(const string &id) -> void;
    auto GetSelectedBody() -> string;

    auto GetMinZoom() -> float;

    auto GetMassiveBodies() -> unordered_map<string, Massive>;
    auto GetMasslessBodies() -> unordered_map<string, Massless>;

    auto GetMassiveBody(const string &id) -> Massive;
    auto GetMasslessBody(const string &id) -> Massless;
}