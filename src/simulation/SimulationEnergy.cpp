#include "SimulationEnergy.h"

#include "Simulation.h"
#include "bodies/Body.h"
#include "main/Bodies.h"


namespace SimulationEnergy {
    namespace {
        auto PotentialEnergy(const Body &ofBody, const Body &withRespectTo) -> double {
            ZoneScoped;
            double massProduct = ofBody.GetMass() * withRespectTo.GetMass();
            double distance = glm::distance(ofBody.GetPosition(), withRespectTo.GetPosition());
            return (GRAVITATIONAL_CONSTANT * massProduct) / distance;
        }
    }

    auto GetKineticEnergy(const Body &body) -> double {
        ZoneScoped;
        return 0.5 * body.GetMass() * pow(glm::length(body.GetVelocity()), 2); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    }

    auto GetPotentialEnergy(const Body &body) -> double {
        ZoneScoped;
        double energy = 0;
        for (const auto &pair : Bodies::GetMassiveBodies()) {
            if (pair.first == body.GetId()) {
                continue;
            }
            energy -= PotentialEnergy(body, pair.second);
        }
        return energy;
    }

    auto GetTotalEnergy(const Body &body) -> double {
        ZoneScoped;
        return GetKineticEnergy(body) + GetPotentialEnergy(body);
    }

    auto GetSimulationKineticEnergy() -> double {
        ZoneScoped;
        double energy = 0;
        for (const auto &pair : Bodies::GetBodies())  { energy += GetKineticEnergy(pair.second); }
        return energy;
    }

    auto GetSimulationPotentialEnergy() -> double {
        ZoneScoped;
        double energy = 0;
        for (const auto &pair : Bodies::GetBodies())  { energy += GetPotentialEnergy(pair.second); }
        return energy;
    }

    auto GetSimulationTotalEnergy() -> double {
        ZoneScoped;
        return GetSimulationKineticEnergy() + GetSimulationPotentialEnergy();
    }
}