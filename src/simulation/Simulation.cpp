#include "Simulation.h"
#include "simulation/SimulationState.h"

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
        const unsigned int INITIAL_SPEED_VALUE = 1;
        const unsigned int INITIAL_SPEED_DEGREE = 1;
        const unsigned int INITIAL_TIME_STEP = 0;
        const unsigned int INITIAL_TIME_SINCE_LAST_STATE_UPDATE = 0;

        const unsigned int SPEED_MULTIPLIER = 4;
        const unsigned int MAX_SPEED_DEGREE = 13;
        const unsigned int MAX_SPEED = 2000000; // (speedMultiplier^maxSpeedDegree) + 1
        const unsigned int MIN_SPEED = 1;
        const unsigned int MAX_FUTURE_STATES = 5000;
        const unsigned int MAX_PAST_STATES = 500;

        const unsigned int TIME_STEP_SIZE = 1000;

        // danger: rawFutureStates and rawPastStates are used by multiple threads, use stateVectorMutex
        std::mutex stateVectorMutex;
        vector<SimulationState> futureStates;
        vector<SimulationState> pastStates;

        double speedValue = INITIAL_SPEED_VALUE;
        double speedDegree = INITIAL_SPEED_DEGREE;

        double timeStep = INITIAL_TIME_STEP;
        double timeSinceLastStateUpdate = INITIAL_TIME_SINCE_LAST_STATE_UPDATE;

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
        
        auto StepBodyToNextState(const string &id) -> void {
            SimulationState nextState = futureStates.at(1);
            OrbitPoint point = nextState.GetOrbitPoints().at(id);
            Bodies::UpdateBody(id, point);
        }

        auto GetInitialState() -> unordered_map<string, OrbitPoint> {
            unordered_map<string, OrbitPoint> initialStateMap;

            for (const auto &pair : Bodies::GetMassiveBodies()) {
                OrbitPoint initialOrbitPoint = OrbitPoint{
                    pair.second.GetPosition(), 
                    pair.second.GetVelocity()};
                initialStateMap.insert(std::make_pair(pair.first, initialOrbitPoint));
            }

            for (const auto &pair : Bodies::GetMasslessBodies()) {
                OrbitPoint initialOrbitPoint = OrbitPoint{
                    pair.second.GetPosition(), 
                    pair.second.GetVelocity()};
                initialStateMap.insert(std::make_pair(pair.first, initialOrbitPoint));
            }

            return initialStateMap;
        }

        auto MoveLatestFutureStateToPastState() -> void {
            std::lock_guard<std::mutex> lock(stateVectorMutex);
            pastStates.push_back(futureStates.at(1));
            futureStates.erase(futureStates.begin());
            if (pastStates.size() > MAX_PAST_STATES) {
                pastStates.erase(pastStates.begin());
            }
        }
    }

    auto Init() -> void {
        Keys::BindFunctionToKeyPress(GLFW_KEY_COMMA, DecreaseSimulationSpeed);
        Keys::BindFunctionToKeyPress(GLFW_KEY_PERIOD, IncreaseSimulationSpeed);
    }

    auto PreReset() -> void {
        std::lock_guard<std::mutex> lock(stateVectorMutex);
        speedValue = INITIAL_SPEED_VALUE;
        speedDegree = INITIAL_SPEED_DEGREE;
        timeStep = INITIAL_TIME_STEP;
        timeSinceLastStateUpdate = INITIAL_TIME_SINCE_LAST_STATE_UPDATE;
        pastStates.clear();
        futureStates.clear();
    }

    auto NewBodyReset() -> void {
        // To be called when a new body is added to the system
        std::lock_guard<std::mutex> lock(stateVectorMutex);
        pastStates.clear();
        futureStates.clear();
        unordered_map<string, OrbitPoint> initialStateMap = GetInitialState();
        SimulationState initialState(initialStateMap);
        futureStates.push_back(initialState);
    }

    auto Update(const double deltaTime) -> void {
        ZoneScoped;
        timeStep += deltaTime * speedValue;
        timeSinceLastStateUpdate += deltaTime * speedValue;

        while ((timeSinceLastStateUpdate >= Simulation::GetTimeStepSize()) && (futureStates.size() > 1)) {
            timeSinceLastStateUpdate -= Simulation::GetTimeStepSize();
            for (const auto &pair : Bodies::GetMassiveBodies())  { StepBodyToNextState(pair.first); }
            for (const auto &pair : Bodies::GetMasslessBodies()) { StepBodyToNextState(pair.first); }
            MoveLatestFutureStateToPastState();
        }

        while (futureStates.size() < MAX_FUTURE_STATES) {
            SimulationState latestState = futureStates.at(futureStates.size()-1);
            latestState.StepToNextState(TIME_STEP_SIZE);
            std::lock_guard<std::mutex> lock(stateVectorMutex);
            futureStates.push_back(latestState);
        }
    }

    auto GetPastStates() -> vector<SimulationState> {
        std::lock_guard<std::mutex> lock(stateVectorMutex);
        return pastStates;
    }

    auto GetFutureStates() -> vector<SimulationState> {
        std::lock_guard<std::mutex> lock(stateVectorMutex);
        return futureStates;
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

    auto GetTimeStep() -> double {
        return timeStep;
    }

    auto SetTimeStep(const double _timeStep) -> void {
        timeStep = _timeStep;
    }
}