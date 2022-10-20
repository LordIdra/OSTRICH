#include "Interface.h"

#include <rendering/interface/OrbitPaths.h>
#include <rendering/interface/MassiveRender.h>
#include <rendering/interface/icons/Icons.h>

#include <glad/glad.h>


namespace Interface {

    auto Init() -> void {
        MassiveRender::Init();
        OrbitPaths::Init();
        Icons::Init();
    }
    
    auto Update(const double deltaTime) -> void {
        // WTF are you doing??? Why disable depth testing?!?!?!
        // The reason is that otherwise, the paths are invariably closer to the camera due to being in screen space
        // This of course causes some issues where the paths draw on top of icons, bodies, and just everything really
        // By disabling depth test, we force OpenGL to ignore the fact that the orbit paths are closer than the
        // other interface elements
        glDisable(GL_DEPTH_TEST);
        OrbitPaths::Update();
        glEnable(GL_DEPTH_TEST);

        MassiveRender::Update(deltaTime);
        Icons::DrawIcons();
    }
}