#include "Buttons.h"

#include <window/Window.h>
#include <util/Log.h>

#include <GLFW/glfw3.h>



// important: all variables set inside callbacks in this namespace are updated when glfwPollEvents() is called
namespace Mouse {

    namespace {
        const float DOUBLE_CLICK_DURATION = 0.5;

        bool leftHeld;
        bool rightHeld;

        bool leftActive;
        double timeOfLastLeftClick; // seconds

        auto DefaultCallback() -> void {}

        vector<void (*)()> leftPress;
        vector<void (*)()> leftRelease;
        vector<void (*)()> leftDouble;

        vector<void (*)()> rightPress;
        vector<void (*)()> rightRelease;

        auto ButtonCallbackPressLeft() -> void {
            // Double click stuff
            if (leftActive) {
                for (void (*function)() : leftDouble) {
                    function();
                }
            } else {
                leftActive = true;
                timeOfLastLeftClick = glfwGetTime();
            }

            // General callback stuff
            leftHeld = true;
            for (void (*function)() : leftPress) {
                function();
            }
        }

        auto ButtonCallbackPressRight() -> void {
            // Mouse should be hidden when right mouse button held
            Mouse::Hide();

            // General callback stuff
            rightHeld = true;
            for (void (*function)() : rightPress) {
                function();
            }
        }

        auto ButtonCallbackReleaseLeft() -> void {
            // General callback stuff
            leftHeld = false;
            for (void (*function)() : leftRelease) {
                function();
            }
        }

        auto ButtonCallbackReleaseRight() -> void {
            // Mouse should be shown again when right mouse button released
            Mouse::Show();

            // General callback stuff
            rightHeld = false;
            for (void (*function)() : rightRelease) {
                function();
            }
        }

        auto ButtonCallbackPress(const int button) -> void {
            switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                ButtonCallbackPressLeft();
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                ButtonCallbackPressRight();
                break;
            }
        }

        auto ButtonCallbackRelease(const int button) -> void {
            switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                ButtonCallbackReleaseLeft();
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                ButtonCallbackReleaseRight();
                break;
            }
        }

        auto ButtonCallback(GLFWwindow* window, int button, int action, int mods) -> void { // NOLINT(misc-unused-parameters)
            switch (action) {
            case GLFW_PRESS:
                ButtonCallbackPress(button);
                break;
            case GLFW_RELEASE:
                ButtonCallbackRelease(button);
                break;
            }
        }
    }

    auto AddCallbackLeftPress(void (*function)()) -> void {
        leftPress.push_back(function);
    }

    auto AddCallbackLeftRelease(void (*function)()) -> void {
        leftRelease.push_back(function);
    }

    auto AddCallbackLeftDouble(void (*function)()) -> void {
        leftDouble.push_back(function);
    }

    auto AddCallbackRightPress(void (*function)()) -> void {
        rightPress.push_back(function);
    }

    auto AddCallbackRightRelease(void (*function)()) -> void {
        rightRelease.push_back(function);
    }

    auto LeftButtonHeld() -> bool {
        return leftHeld;
    }

    auto RightButtonHeld() -> bool {
        return rightHeld;
    }

    auto Show() -> void{
        glfwSetInputMode(Window::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    auto Hide() -> void{
        glfwSetInputMode(Window::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    }
}