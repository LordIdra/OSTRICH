#include "Window.h"

#include <util/Log.h>

#include <string>



namespace Window {

    namespace {
        GLFWmonitor *monitor;
        GLFWwindow *window;
        const GLFWvidmode *mode;

        int width;
        int height;

        auto FramebufferSizeCallback(GLFWwindow* window, int width_, int height_) -> void { //NOLINT(misc-unused-parameters)
            // This function is called whenever the window is resized
            // Resize the viewport (the range in which OpenGL draws geometry) and update the width/height
            glViewport(0, 0, width_, height_);
            width = width_;
            height = height_;
        }

        auto AcquireMonitorAndMode() -> void {
            // Monitor/mode are important for other stuff later on
            monitor = glfwGetPrimaryMonitor();
            mode = glfwGetVideoMode(monitor);
            width = mode->width;
            height = mode->height;
        }

        void SetWindowHints(const bool fullscreen) {
            // If we're in fullscreen mode, disabling GLFW_DECORATED prevents flashing when the window is initially created
            if (fullscreen) {
                glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            }
        }

        auto CreateWindow(const string &title, const bool fullscreen) -> void {
            // If we're in fullscreen, acquire a monitor - if not, just set the monitor variable to null
            GLFWmonitor *windowMonitor = fullscreen ? monitor : nullptr;

            // If the monitor is null, this function will create a windowed window
            // If the monitor isn't null, it'll create a fullscreen window
            window = glfwCreateWindow(width, height, title.c_str(), windowMonitor, nullptr);

            // Now we can show the window to the user and set the function called when the window is resized
            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
        }
    }


    auto Init(const string &title) -> void {
        AcquireMonitorAndMode();
        SetWindowHints(false);
        CreateWindow(title, false);
    }

    auto Update() -> void {
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    auto Background(const vec4 color) -> void {
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

