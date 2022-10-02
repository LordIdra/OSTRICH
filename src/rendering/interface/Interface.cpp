#include "Interface.h"
#include "rendering/interface/icons/Icons.h"


namespace Interface {

    auto Init() -> void {
        Icons::Init();
    }
    
    auto Update() -> void {
        Icons::DrawIcons();
    }

}