#pragma once

#include <util/Types.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>



namespace Window {
    auto Init(const bool fullscreen, const string &title) -> void;
    auto Update() -> void;

    auto Background(vec4 color) -> void;

    auto GetWidth() -> unsigned int;
    auto GetHeight() -> unsigned int;

    auto ShowCursor() -> void;
    auto HideCursor() -> void;

    auto Close() -> void;
    auto ShouldClose() -> bool;

    auto GetWindow() -> GLFWwindow*;
};
