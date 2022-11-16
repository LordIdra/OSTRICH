#pragma once

#include "bodies/Body.h"



namespace SimulationEnergy {

    auto GetKineticEnergy(const Body &body) -> double;
    auto GetPotentialEnergy(const Body &body) -> double;
    auto GetTotalEnergy(const Body &body) -> double;

    auto GetSimulationKineticEnergy() -> double;
    auto GetSimulationPotentialEnergy() -> double;
    auto GetSimulationTotalEnergy() -> double;
}