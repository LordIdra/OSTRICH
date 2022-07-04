#include "control.h"
#include "../util/logging.h"
#include "../window/window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>



namespace control {
    namespace {
        unsigned int MAJOR_VERSION = 3;
        unsigned int MINOR_VERSION = 3;

        auto SetVersionHints() -> void {
            // tell OpenGL which version to use, and tell OpenGL to use the CORE profile rather than COMPATIBILITY
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        auto InitGLFW() -> void {
            if (!glfwInit()) {
                logging::Error("Failed to initialize GLFW.");
                return;
            }
            SetVersionHints();
        }

        auto InitGlad() -> void {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {  //NOLINT - required cast for GLAD initialization
                logging::Error("Failed to initialize GLAD.");
                return;
            }
            // depth testing makes sure that fragments closer to the camera override fragments further away
            glEnable(GL_DEPTH_TEST);
        }
    }


    auto Init(const string &windowTitle) -> void {
        InitGLFW();
        window::Init(windowTitle);
        InitGlad();
    }

    auto Update() -> void {
        window::Update();
    }
}

