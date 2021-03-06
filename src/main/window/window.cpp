#include "window.h"

#include "../util/Types.h"
#include "../util/logging.h"



namespace window {

    namespace {
        GLFWmonitor *monitor;
        GLFWwindow *window;
        const GLFWvidmode *mode;

        unsigned int width;
        unsigned int height;

        auto FramebufferSizeCallback(GLFWwindow* window, int width, int height) -> void {
            // this function is called whenever the window is resized
            glViewport(0, 0, width, height);
            // TODO check if we need to update width/height
        }

        auto AcquireMonitorAndMode() -> void {
            // monitor/mode are important for other stuff later on
            monitor = glfwGetPrimaryMonitor();
            mode = glfwGetVideoMode(monitor);
            width = mode->width;
            height = mode->height;
        }

        void SetWindowHints() {
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // this prevents flashing when the window is initially created
        }

        auto CreateWindow(const string &title, const bool fullscreen) -> void {
            GLFWmonitor *windowMonitor = fullscreen ? monitor : nullptr;
            window = glfwCreateWindow(width, height, title.c_str(), windowMonitor, nullptr);
            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
        }
    }


    auto Init(const string &title) -> void {
        AcquireMonitorAndMode();
        SetWindowHints();
        CreateWindow(title, false);
    }

    auto Update() -> void {
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    auto Background(vec4 color) -> void {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    auto Close() -> void {
        glfwWindowShouldClose(window);
    }

    auto ShouldClose() -> bool {
        return glfwWindowShouldClose(window);
    }

    auto GetWindow() -> GLFWwindow* {
        return window;
    }

    auto GetWidth() -> unsigned int {
        return width;
    }

    auto GetHeight() -> unsigned int {
        return height;
    }
};

