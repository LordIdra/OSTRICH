#include "OrbitPaths.h"

#include <GL/gl.h>
#include <simulation/OrbitPoint.h>
#include "rendering/VAO.h"
#include "simulation/SimulationState.h"
#include "util/Constants.h"
#include "util/Log.h"

#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <rendering/shaders/Program.h>
#include <rendering/geometry/Rays.h>
#include <rendering/camera/Camera.h>
#include <string>
#include <unordered_map>
#include <util/Types.h>
#include <main/Bodies.h>

#include <utility>

using std::unique_ptr;



namespace OrbitPaths {

    namespace {

        const unsigned int MAX_FUTURE_STATES = 5000;
        const unsigned int MAX_PAST_STATES = 500;

        const int STRIDE = 6;
        const float PATH_WIDTH = 0.005;
        const vec3 PATH_COLOR = vec3(0.0, 0.0, 0.9);
        const float DISTANCE_THRESHOLD = 0.01;
        const float RATIO_OF_FUTURE_POINTS_TO_FADE_OVER = 0.05;
        const float PAST_POINT_BRIGHTNESS = 0.4;

        std::mutex threadMutex;

        int pastVerticesToRemoveNextFramePerBody = 0;
        int futureVerticesToRemoveNextFrame = 0;

        unique_ptr<VAO> futurePointsVAO;
        unique_ptr<VAO> pastPointsVAO;
        unique_ptr<Program> program;

        vector<VERTEX_DATA_TYPE> futureVertices;
        unordered_map<string, vector<VERTEX_DATA_TYPE>> pastVertices;

        auto RemoveScheduledVertices() -> void {
            ZoneScoped;
            std::lock_guard<std::mutex> lock(threadMutex);
            for (auto &pair : pastVertices) {
                pair.second.erase(pair.second.begin(), pair.second.begin() + pastVerticesToRemoveNextFramePerBody*STRIDE);
            }
            futureVertices.erase(futureVertices.begin(), futureVertices.begin() + futureVerticesToRemoveNextFrame*STRIDE);
        }

        auto ScaleStateMap(vector<SimulationState> &unscaledPointMap) -> void {
            ZoneScoped;
            for (SimulationState &state : unscaledPointMap) {
                state.Scale();
            }
        }

        auto AddVertex(vector<VERTEX_DATA_TYPE> &vertices, const vec3 position, const vec3 color) -> void {
            ZoneScoped;
            vertices.push_back(position.x);
            vertices.push_back(position.y);
            vertices.push_back(position.z);
            vertices.push_back(color.r);
            vertices.push_back(color.g);
            vertices.push_back(color.b);
        }

        auto DrawFuturePoints(const unsigned int drawMethod) -> void {
            ZoneScoped;
            program->Use();
            program->Set("cameraMatrix", Camera::GetMatrix());
            futurePointsVAO->Data(futureVertices, futureVertices.size() / STRIDE, GL_DYNAMIC_DRAW);
            futurePointsVAO->Render(drawMethod);
        }

        auto DrawPastPoints(const unsigned int drawMethod) -> void {
            ZoneScoped;
            program->Use();
            program->Set("cameraMatrix", Camera::GetMatrix());

            for (const auto &pair : pastVertices) {
                pastPointsVAO->Data(pair.second, pair.second.size() / STRIDE, GL_DYNAMIC_DRAW);
                pastPointsVAO->Render(drawMethod);
            }
        }
    }

    auto Init() -> void {
        // Create Program
        Shader vertex = Shader("../resources/shaders/path-vertex.vsh", GL_VERTEX_SHADER);
        Shader fragment = Shader("../resources/shaders/path-fragment.fsh", GL_FRAGMENT_SHADER);
        program = std::make_unique<Program>(vertex, fragment);

        // Create future points VAO
        futurePointsVAO = std::make_unique<VAO>();
        futurePointsVAO->Init();
        futurePointsVAO->AddVertexAttribute(
            VertexAttribute{
            .index = 0,
            .size = 3,
            .type = GL_FLOAT,
            .normalised = GL_FALSE,
            .stride = STRIDE * sizeof(float),
            .offset = nullptr});
        futurePointsVAO->AddVertexAttribute(VertexAttribute{
            .index = 1,
            .size = 3,
            .type = GL_FLOAT,
            .normalised = GL_FALSE,
            .stride = STRIDE * sizeof(float),
            .offset = (void*)(3 * sizeof(float))}); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)

        // Create past points VAO
        pastPointsVAO = std::make_unique<VAO>();
        pastPointsVAO->Init();
        pastPointsVAO->AddVertexAttribute(
            VertexAttribute{
            .index = 0,
            .size = 3,
            .type = GL_FLOAT,
            .normalised = GL_FALSE,
            .stride = STRIDE * sizeof(float),
            .offset = nullptr});
        pastPointsVAO->AddVertexAttribute(VertexAttribute{
            .index = 1,
            .size = 3,
            .type = GL_FLOAT,
            .normalised = GL_FALSE,
            .stride = STRIDE * sizeof(float),
            .offset = (void*)(3 * sizeof(float))}); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
    }

    auto NewBodyReset() -> void {
        std::lock_guard<std::mutex> lock(threadMutex);
        futureVertices = vector<VERTEX_DATA_TYPE>();

        pastVertices.clear();
        for (const auto &pair : Bodies::GetBodies()) {
            pastVertices.insert(std::make_pair(pair.first, vector<VERTEX_DATA_TYPE>()));
        }
    }

    auto Update() -> void {
        ZoneScoped;
        RemoveScheduledVertices();
        futureVerticesToRemoveNextFrame = 0;
        pastVerticesToRemoveNextFramePerBody = 0;
        DrawFuturePoints(GL_POINTS);
        DrawPastPoints(GL_LINE_STRIP);
    }

    auto AddNewState(const SimulationState &state) -> void {
        ZoneScoped;
        // Add new state to future vertices
        std::lock_guard<std::mutex> lock(threadMutex);
        for (const auto &pair : state.GetOrbitPoints()) {
            AddVertex(futureVertices, Rays::Scale(pair.second.position), Bodies::GetBody(pair.first).GetColor());
        }
    }

    auto StepToNextState(const SimulationState &state) -> void {
        ZoneScoped;
        std::lock_guard<std::mutex> lock(threadMutex);
        // The first vertices are now past points, so move them to the past points vector
        bool deletePastVertex = false;
        for (const auto &pair : state.GetOrbitPoints()) {
            ZoneScoped;
            vector<VERTEX_DATA_TYPE> &pastBodyVertices = pastVertices.at(pair.first);
            const Body &body = Bodies::GetBody(pair.first);

            AddVertex(
                pastBodyVertices, 
                Rays::Scale(pair.second.position), 
                body.GetColor());

            // If there are too many vertices in the past vertex vector, remove the first element of it (tail of the path)
            if (pastBodyVertices.size() > MAX_PAST_STATES) {
                deletePastVertex = true;
            }

            // Remove the first element of the future vertex map (the one we just moved to)
            futureVerticesToRemoveNextFrame++;
        }

        if (deletePastVertex) {
            pastVerticesToRemoveNextFramePerBody++;
        }
    }

    auto GetMaxFutureStates() -> unsigned int {
        return MAX_FUTURE_STATES;
    }
}