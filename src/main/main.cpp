#include "window/window.h"
#include "control/control.h"


int main() {

    control::Init("TEST!");

    while (!window::ShouldClose()) {
        window::Background(vec4(0.4, 0.0, 0.0, 1.0));
        control::Update();
    }

    // 0 = successful execution
    return 0;
}
