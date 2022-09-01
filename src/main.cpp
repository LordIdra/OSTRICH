#include <bodies/Massive.h>
#include <control/Control.h>

#include <input/Keys.h>
#include <input/Mouse.h>

#include <rendering/shaders/Program.h>
#include <rendering/VAO.h>
#include <rendering/camera/Camera.h>
#include <rendering/Geometry.h>

#include <util/Log.h>
#include <util/Types.h>

#include <rendering/geometry/Rays.h>
#include <rendering/geometry/Transition.h>

#include <window/Window.h>

#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>



Massive earth(
    bvec3(0, 0, 0),
    bvec3(0, 0, 0),
    "Earth",
    bfloat(5.9722e24),
    bfloat(6371.0e3)
);

Massive moon(
    bvec3(bfloat(0.4055e8), bfloat(0), bfloat(0)),
    bvec3(bfloat(0), bfloat(0), bfloat(0.970e3)),
    "The Moon",
    bfloat(0.07346e24),
    bfloat(1737.4e3)
);

const Material earthMaterial {
    .ambient = vec3(0.02, 0.05, 0.07),
    .diffuse = vec3(0.1, 0.25, 0.35),
    .specular = vec3(0.2, 0.5, 0.7),
    .shine = 64.0
};

const Material moonMaterial {
    .ambient = vec3(0.04, 0.04, 0.04),
    .diffuse = vec3(0.3, 0.3, 0.3),
    .specular = vec3(0.6, 0.6, 0.6),
    .shine = 32.0
};

const Material selectedMaterial {
    .ambient = vec3(0.8, 0.0, 0.0),
    .diffuse = vec3(0.0, 0.0, 0.0),
    .specular = vec3(0.0, 0.0, 0.0),
    .shine = 32.0
};

const float KEY_ZOOM_AMOUNT = 0.5;
Program program;
VAO vao1;
VAO vao2;

Massive *selectedBody = &earth;

vec3 defaultVector = vec3(0, 0, 0);
Transition transition = Transition(defaultVector, defaultVector, 0.0);

auto KeyZoomIn() -> void {
    Camera::AddZoomDelta(KEY_ZOOM_AMOUNT);
}

auto KeyZoomOut() -> void {
    Camera::AddZoomDelta(-KEY_ZOOM_AMOUNT);
}

auto SetupVAO(VAO &vao) -> void {
    vao.Init();
    vao.AddVertexAttribute(VertexAttribute{
        .index = 0,
        .size = 3,
        .type = GL_FLOAT,
        .normalised = GL_FALSE,
        .stride = 6 * sizeof(float),
        .offset = nullptr});
    vao.AddVertexAttribute(VertexAttribute{
        .index = 1,
        .size = 3,
        .type = GL_FLOAT,
        .normalised = GL_FALSE,
        .stride = 6 * sizeof(float),
        .offset = (void*)(3 * sizeof(float))});
}

auto AddMassiveToVAO(VAO &vao, Massive &body) -> void {
    vector<VERTEX_DATA_TYPE> data = body.GetSphereVertices();
    unsigned int vertexCount = data.size() / 6;
    vao.Data(data, vertexCount,  GL_STATIC_DRAW);
}

auto SwitchSelectedBody() -> void {
    vec3 direction = Rays::ScreenToWorld(Mouse::GetPosition());

    if (Rays::IntersectsSphere(
        Camera::GetPosition(), 
        direction, 
        earth.ScaledPosition(), 
        earth.ScaledRadius())) {
            selectedBody = &earth;
            transition = Transition(Camera::GetTarget(), earth.ScaledPosition(), 0.2);
    }

    if (Rays::IntersectsSphere(
        Camera::GetPosition(), 
        direction, 
        moon.ScaledPosition(), 
        moon.ScaledRadius())) {
            selectedBody = &moon;
            transition = Transition(Camera::GetTarget(), moon.ScaledPosition(), 0.2);
    }
}

auto InitialiseVAOs() -> void {
    SetupVAO(vao1);
    AddMassiveToVAO(vao1, earth);
    SetupVAO(vao2);
    AddMassiveToVAO(vao2, moon);

}

auto InitialiseProgram() -> void {
    Shader vertex;
    Shader fragment;
    vertex.Init("../resource/vertex.vsh", GL_VERTEX_SHADER);
    fragment.Init("../resource/fragment.fsh", GL_FRAGMENT_SHADER);
    program.Init();
    program.AddShader(vertex);
    program.AddShader(fragment);
    program.Link();
}

auto InitialiseCamera() -> void {
    Camera::SetTarget(vec3(0.0f, 0.0f, 0.0f));
}

auto InitialiseInput() -> void {
    Keys::BindFunctionToKeyHold(GLFW_KEY_EQUAL, KeyZoomIn);
    Keys::BindFunctionToKeyHold(GLFW_KEY_MINUS, KeyZoomOut);
    Mouse::SetCallbackLeftDouble(SwitchSelectedBody);

}

auto WireFrame(bool enabled) -> void {
    if (enabled) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

auto main() -> int {
    Control::Init("TEST!");

    InitialiseProgram();
    InitialiseVAOs();
    InitialiseCamera();
    InitialiseInput();

    float previousTime = 0;
    float deltaTime = 0;

    //WireFrame(true);

    while (!Window::ShouldClose()) {

        deltaTime = glfwGetTime() - previousTime;
        previousTime = glfwGetTime();

        Window::Background(vec4(0.0, 0.0, 0.0, 1.0));
        Camera::AddZoomDelta(Mouse::GetScrollDelta().y);

        if (Mouse::RightButtonHeld()) {
            Camera::AddAngleDelta(Mouse::GetPositionDelta());
        }

        vec3 light_position(-0.02, 0.0, 0.0);

        program.Use();
        program.Set("cameraMatrix", Camera::GetMatrix());
        program.Set("cameraPosition", Camera::GetPosition());
        program.Set("lightPosition", light_position);
        
        if (selectedBody->GetName() == earth.GetName()) {
            program.Set("material", selectedMaterial);
        } else {
            program.Set("material", earthMaterial);
        }
        vao1.Render();

        if (selectedBody->GetName() == moon.GetName()) {
            program.Set("material", selectedMaterial);
        } else {
            program.Set("material", moonMaterial);
        }
        vao2.Render();

        if (!transition.Finished()) {
            Camera::SetTarget(transition.Step(deltaTime));
        }
        
        Control::Update();
    }

    // 0 = successful execution
    return 0;
}
