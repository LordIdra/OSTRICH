#pragma once

#include "../util/Types.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>



namespace window {
    auto Init(const string &title) -> void;
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
