#pragma once

#include <util/Types.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>



namespace Control {
    auto Init(const string &windowTitle) -> void;
    auto Update() -> void;
}

