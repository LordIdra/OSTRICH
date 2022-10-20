#include "Interface.h"

#include <rendering/interface/MassiveRender.h>
#include <rendering/interface/icons/Icons.h>


namespace Interface {

    auto Init() -> void {
        Icons::Init();
        MassiveRender::Init();
    }
    
    auto Update(const double deltaTime) -> void {
        Icons::DrawIcons();
        MassiveRender::Update(deltaTime);
    }

}