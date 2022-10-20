#include "Interface.h"

#include <rendering/interface/OrbitPaths.h>
#include <rendering/interface/MassiveRender.h>
#include <rendering/interface/icons/Icons.h>

#include <glad/glad.h>


namespace Interface {

    auto Init() -> void {
        OrbitPaths::Init();
        Icons::Init();
        MassiveRender::Init();
    }
    
    auto Update(const double deltaTime) -> void {
        OrbitPaths::Update();
        Icons::DrawIcons();
        MassiveRender::Update(deltaTime);
    }
}