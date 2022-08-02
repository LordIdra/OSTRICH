#include "bodies/Massive.h"
#include "control/control.h"

#include "input/keys.h"
#include "input/mouse.h"

#include "rendering/Program.h"
#include "rendering/VAO.h"
#include "rendering/camera.h"

#include "rendering/geometry.h"
#include "util/Types.h"
#include "util/logging.h"
#include "util/rays.h"
#include "window/window.h"

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

auto KeyZoomIn() -> void {
    camera::AddZoomDelta(KEY_ZOOM_AMOUNT);
}

auto KeyZoomOut() -> void {
    camera::AddZoomDelta(-KEY_ZOOM_AMOUNT);
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

auto InitialiseVAOs() -> void {
    SetupVAO(vao1);
    AddMassiveToVAO(vao1, earth);
    SetupVAO(vao2);
    AddMassiveToVAO(vao2, moon);

}

auto InitialiseProgram() -> void {
    Shader vertex;
    Shader fragment;
    vertex.Init("../../resource/vertex.vsh", GL_VERTEX_SHADER);
    fragment.Init("../../resource/fragment.fsh", GL_FRAGMENT_SHADER);
    program.Init();
    program.AddShader(vertex);
    program.AddShader(fragment);
    program.Link();
}

auto InitialiseCamera() -> void {
    camera::SetTarget(vec3(0.0f, 0.0f, 0.0f));
}

auto InitialiseInput() -> void {
    keys::BindFunctionToKeyHold(GLFW_KEY_EQUAL, KeyZoomIn);
    keys::BindFunctionToKeyHold(GLFW_KEY_MINUS, KeyZoomOut);

}

auto WireFrame(bool enabled) -> void {
    if (enabled) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

auto main() -> int {
    control::Init("TEST!");

    InitialiseProgram();
    InitialiseVAOs();
    InitialiseCamera();
    InitialiseInput();

    //WireFrame(true);

    while (!window::ShouldClose()) {
        window::Background(vec4(0.0, 0.0, 0.0, 1.0));
        camera::AddZoomDelta(mouse::GetScrollDelta().y);

        if (mouse::LeftButtonHeld()) {
            camera::AddAngleDelta(mouse::GetPositionDelta());
        }
        

        vec3 light_position(-0.02, 0.0, 0.0);

        program.Use();
        program.Set("cameraMatrix", camera::GetMatrix());
        program.Set("cameraPosition", camera::GetPosition());
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

        vec3 direction = rays::ScreenToWorld(mouse::GetPosition());

        if (rays::IntersectsSphere(
            camera::GetPosition(), 
            direction, 
            earth.ScaledPosition(), 
            earth.ScaledRadius())) {
                selectedBody = &earth;
        }
        
        if (rays::IntersectsSphere(
            camera::GetPosition(), 
            direction, 
            moon.ScaledPosition(), 
            moon.ScaledRadius())) {
                selectedBody = &moon;
        }
        
        control::Update();
    }

    // 0 = successful execution
    return 0;
}
