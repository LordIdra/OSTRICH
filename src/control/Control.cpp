#include "Control.h"

#include <input/Keys.h>
#include <input/Mouse.h>
#include <rendering/camera/Camera.h>
#include <util/Log.h>
#include <window/Window.h>



namespace Control {
    namespace {
        const int MAJOR_VERSION = 3;
        const int MINOR_VERSION = 3;

        auto SetVersionHints() -> void {
            // Tell OpenGL which version to use, and tell OpenGL to use the CORE profile rather than COMPATIBILITY
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        auto InitGLFW() -> void {
            // Try and initialize GLFW, if it fails log a message
            if (!glfwInit()) {
                Log(ERROR, "Failed to initialize GLFW.");
                return;
            }
            SetVersionHints();
        }

        auto InitGlad() -> void {
            // Try and initialize GLAD, if it fails log a message
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {  // NOLINT - required cast for GLAD initialization
                Log(ERROR, "Failed to initialize GLAD.");
                return;
            }

            // Depth testing makes sure that fragments closer to the camera override fragments further away
            glEnable(GL_DEPTH_TEST);
        }
    }

    auto Init(const bool fullscreen, const string &windowTitle) -> void {
        InitGLFW();
        Window::Init(fullscreen, windowTitle);
        InitGlad();
        Keys::Init();
        Mouse::Init();
    }

    auto Update() -> void {
        Mouse::Update();
        Keys::Update();
        glfwPollEvents();
        Window::Update();
        Camera::Update();
    }
}

