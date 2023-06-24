#include "Simulation.h"
#include "simulation/SimulationState.h"

#include <rendering/world/OrbitPaths.h>
#include <bodies/Body.h>
#include <glm/gtx/string_cast.hpp>
#include <input/Keys.h>
#include <main/Bodies.h>
#include <mutex>
#include <simulation/OrbitPoint.h>

#include <GLFW/glfw3.h>
#include <string>
#include <tracy/Tracy.hpp>
#include <unordered_map>
#include <utility>
#include <vector>



namespace Simulation {

    namespace {
        const unsigned int STATE_CACHE_RESERVE = 128;
        const unsigned int POINT_RENDER_INTERVAL = 5;

        const unsigned int INITIAL_SPEED_VALUE = 1;
        const unsigned int INITIAL_SPEED_DEGREE = 1;
        const unsigned int INITIAL_TIME_STEP = 0;
        const unsigned int INITIAL_TIME_SINCE_LAST_STATE_UPDATE = 0;

        const unsigned int SPEED_MULTIPLIER = 4;
        const unsigned int MAX_SPEED_DEGREE = 12;
        const unsigned int MAX_SPEED = 2000000;
        const unsigned int MIN_SPEED = 1;

        const unsigned int TIME_STEP_SIZE = 10000;

        std::atomic_bool terminateUpdate = false;


        // important: staticState is updated at the end of each frame and represents a snapshot of the simulation
        // important: This prevents incoherent states, since the state might be changed over multiple update calls to other functions
        // important: staticState should be the ONLY state container that can be queried or modified from outside the update thread
        std::mutex stateMutex;
        SimulationState staticState;
        SimulationState state;
        SimulationState futureState;

        vector<SimulationState> stateCache;

        double speedValue = INITIAL_SPEED_VALUE;
        double speedDegree = INITIAL_SPEED_DEGREE;

        double timeStep = INITIAL_TIME_STEP;
        double timeSinceLastStateUpdate = INITIAL_TIME_SINCE_LAST_STATE_UPDATE;
        
        unsigned int futureStep = 0;

        auto ShouldNewStateBeAdded() -> bool {
            // Returns true every 'POINT_RENDER_INTERVAL'th time it is called
            static int x = 0;
            x++;
            if (x >= POINT_RENDER_INTERVAL) {
                x = 0;
                return true;
            }
            return false;
        }

        auto ShouldNewStateBeRendered() -> bool {
            // Returns true every 'POINT_RENDER_INTERVAL'th time it is called
            static int x = 0;
            x++;
            if (x >= POINT_RENDER_INTERVAL) {
                x = 0;
                return true;
            }
            return false;
        }

        auto IncreaseSimulationSpeed() -> void {
            if (speedValue < MAX_SPEED) {
                speedDegree += 1;
                speedValue *= SPEED_MULTIPLIER;
            }
        }

        auto DecreaseSimulationSpeed() -> void {
            if (speedValue > MIN_SPEED) {
                speedDegree -= 1;
                speedValue /= SPEED_MULTIPLIER;
            }
        }

        auto AcquireInitialState() -> SimulationState {
            unordered_map<string, OrbitPoint> initialStateMap;

            for (const auto &pair : Bodies::GetBodies()) {
                OrbitPoint initialOrbitPoint{
                    pair.second.GetPosition(), 
                    pair.second.GetVelocity()};
                initialStateMap.insert(std::make_pair(pair.first, initialOrbitPoint));
            }

            return initialStateMap;
        }

        auto UpdateState() -> void {
            while (timeSinceLastStateUpdate >= Simulation::GetTimeStepSize()) {
                ZoneNamedN(STEP, "Step to next state", true);
                timeSinceLastStateUpdate -= Simulation::GetTimeStepSize();
                state.StepToNextState(TIME_STEP_SIZE);
                ZoneNamedN(BODY_STEP, "Step bodies to next state", true);
                stateCache.push_back(state);
                futureStep--;
            }
        }

        auto UpdateFutureState() -> void {
            ZoneScoped;
            terminateUpdate = false;
            while ((futureStep < OrbitPaths::GetMaxFutureStates()) && (!terminateUpdate)) {
                futureState.StepToNextState(TIME_STEP_SIZE);
                if (ShouldNewStateBeAdded()) {
                    OrbitPaths::AddNewState(futureState);
                }
                futureStep++;
            }
        }
    }

    auto Init() -> void {
        Keys::BindFunctionToKeyPress(GLFW_KEY_COMMA, DecreaseSimulationSpeed);
        Keys::BindFunctionToKeyPress(GLFW_KEY_PERIOD, IncreaseSimulationSpeed);
        stateCache.reserve(STATE_CACHE_RESERVE);
    }

    auto PreReset() -> void {
        speedValue = INITIAL_SPEED_VALUE;
        speedDegree = INITIAL_SPEED_DEGREE;
        timeStep = INITIAL_TIME_STEP;
        timeSinceLastStateUpdate = INITIAL_TIME_SINCE_LAST_STATE_UPDATE;
        futureStep = 0;
    }

    auto NewBodyReset() -> void {
        // To be called when a new body is added to the system
        std::lock_guard<std::mutex> lock(stateMutex);
        staticState = futureState = state = AcquireInitialState();
    }

    auto FrameUpdate() -> void {
        std::lock_guard<std::mutex> lock(stateMutex);
        staticState = state;

        // Update all the paths
        // If we did this in the main update function, the paths would be indepedently updated from the other update functions
        // So depending on where the update function was called in the frame, we might end up with an inconsistent state
        // where the orbit paths indicate the body is somewhere else
        for (SimulationState state : stateCache) {
            if (ShouldNewStateBeRendered()) {
                OrbitPaths::StepToNextState(state);
            }
        }

        // Now update the body to correspond to the latest state
        if (!stateCache.empty()) {
            SimulationState latestState = stateCache.at(stateCache.size()-1);
            for (const auto &pair : latestState.GetOrbitPoints())
            Bodies::UpdateBody(pair.first, pair.second);
        }

        stateCache.clear();
    }

    auto Update(const double deltaTime) -> void {
        // This function is run async to basically everything else
        ZoneScoped;
        timeStep += deltaTime * speedValue;
        timeSinceLastStateUpdate += deltaTime * speedValue;
        UpdateState();
        UpdateFutureState();        
    }

    auto TerminateUpdate() -> void {
        // Signal that we need to finish updating and prepare for the thread to be joined
        terminateUpdate = true;
    }

    auto GetSpeedValue() -> double {
        return speedValue;
    }

    auto GetSpeedDegree() -> double {
        return speedDegree;
    }

    auto GetMaxSpeedDegree() -> unsigned int {
        return MAX_SPEED_DEGREE;
    }

    auto GetTimeStepSize() -> unsigned int {
        return TIME_STEP_SIZE;
    }

    auto GetAcceleration(const string &id) -> dvec3 {
        return staticState.CalculateTotalAcceleration(id);
    }

    auto GetTimeStep() -> double {
        return timeStep;
    }

    auto SetTimeStep(const double _timeStep) -> void {
        timeStep = _timeStep;
    }
}