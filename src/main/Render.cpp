#include "Render.h"

#include <memory>
#include <rendering/geometry/Transition.h>
#include <rendering/camera/Camera.h>
#include <rendering/shaders/Program.h>
#include <rendering/VAO.h>
#include <util/Types.h>
#include <input/Mouse.h>
#include <input/Keys.h>
#include <main/Materials.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using std::unique_ptr;
using std::make_unique;



namespace Render {
    namespace {
        const float KEY_ZOOM_AMOUNT = 0.2;
        const vec3 ZERO_VECTOR = vec3(0, 0, 0);
        const vec3 LIGHT_POSITION = vec3(-0.02, 0.0, 0.0);
        const unsigned int STRIDE = 6;
        const float TRANSITION_TIME = 0.2;

        unordered_map<string, VAO> vaos;
        Transition transition = Transition(ZERO_VECTOR, ZERO_VECTOR, 0.0);
        unique_ptr<Program> program;


        auto KeyZoomIn() -> void {
            Camera::AddZoomDelta(KEY_ZOOM_AMOUNT);
        }

        auto KeyZoomOut() -> void {
            Camera::AddZoomDelta(-KEY_ZOOM_AMOUNT);
        }

        auto WireFrame(bool enabled) -> void {
            glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
        }

        auto AddVertexAttributes(VAO &vao) -> void {
            vao.AddVertexAttribute(VertexAttribute{
                .index = 0,
                .size = 3,
                .type = GL_FLOAT,
                .normalised = GL_FALSE,
                .stride = STRIDE * sizeof(float),
                .offset = nullptr});
            vao.AddVertexAttribute(VertexAttribute{
                .index = 1,
                .size = 3,
                .type = GL_FLOAT,
                .normalised = GL_FALSE,
                .stride = STRIDE * sizeof(float),
                .offset = (void*)(3 * sizeof(float))}); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
        }
    }

    auto Init() -> void {
        // Wireframe
        WireFrame(false);

        // Input
        Keys::BindFunctionToKeyHold(GLFW_KEY_EQUAL, KeyZoomIn);
        Keys::BindFunctionToKeyHold(GLFW_KEY_MINUS, KeyZoomOut);
        
        // Program
        Shader vertex = Shader("../resource/vertex.vsh", GL_VERTEX_SHADER);
        Shader fragment = Shader("../resource/fragment.fsh", GL_FRAGMENT_SHADER);
        program = make_unique<Program>(vertex, fragment);
    }

    auto Update(double deltaTime) -> void {
        // Set program variables
        program->Use();
        program->Set("cameraMatrix", Camera::GetMatrix());
        program->Set("cameraPosition", Camera::GetPosition());
        program->Set("lightPosition", LIGHT_POSITION);
        program->Set("material", planetMaterial);

        // Render every VAO
        for (const auto &pair: vaos) {
            pair.second.Render();
        }

        // Update the camera target according to the transition
        if (!transition.Finished()) {
            Camera::SetTarget(transition.Step(deltaTime));
        }
    }

    auto AddBody(const Massive &body) -> void {
        // Add a corresponding VAO for the body
        vaos.insert(std::make_pair(body.GetId(), VAO()));
        VAO &vao = vaos.at(body.GetId());
        vao.Init();

        // Add VAO vertex attributes
        AddVertexAttributes(vao);

        // Set VAO data
        vector<VERTEX_DATA_TYPE> data = body.GetSphereVertices();
        unsigned int vertexCount = data.size() / STRIDE;
        vao.Data(data, vertexCount,  GL_STATIC_DRAW);
    }

    auto StartTransition(const Massive &body) -> void {
        transition = Transition(Camera::GetTarget(), body.GetScaledPosition(), TRANSITION_TIME);
    }
}