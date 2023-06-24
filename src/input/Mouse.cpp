#include "Mouse.h"

#include "mouse/Buttons.cpp"
#include "mouse/Position.cpp"
#include "mouse/Scroll.cpp"

#include <window/Window.h>



namespace Mouse {
    auto Init() -> void {
        // Buttons
        glfwSetMouseButtonCallback(Window::GetWindow(), ButtonCallback);

        // Position
        glfwSetCursorPosCallback(Window::GetWindow(), MoveCallback);

        // Scroll
        glfwSetScrollCallback(Window::GetWindow(), ScrollCallback);
    }

    auto Update() -> void {
        ZoneScoped;
        // Buttons
        if (leftActive) {
            if (glfwGetTime() - timeOfLastLeftClick > DOUBLE_CLICK_DURATION) {
                leftActive = false;
            }
        }

        // Position
        positionDelta = vec2(0.0F, 0.0F);

        // Scroll
        scrollDelta = vec2(0.0F, 0.0F);
    }
}