#include "Scroll.h"

#include <window/Window.h>



namespace Mouse {

    namespace {
        vec2 scrollDelta;

        auto ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) -> void { // NOLINT(misc-unused-parameters)
            // Calculate how far the scroll wheel has moved from its last position
            scrollDelta = glm::vec2(xoffset, yoffset);
        }
    }

    auto GetScrollDelta() -> glm::vec2 {
        // First vale = scrolling to the side
        // Second value = normal scrolling
        return scrollDelta;
    }
}