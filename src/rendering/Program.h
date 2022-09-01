#pragma once

#include "../util/Types.h"
#include "Material.h"
#include "Shader.h"



class Program {
private:
    unsigned int id;

    auto CheckLinkSuccess() const -> void;
    auto PrintLinkLog() const -> void;

public:
    Program();
    
    // the initializer does nothing but generate a program - linking will come later
    auto Init() -> void;

    // add a shader to the program
    auto AddShader(const Shader &shader) const -> void;

    // only call this after adding all the shaders 
    auto Link() -> void;

    // mark the program to be used in subsequent render calls
    auto Use() const -> void;

    // lots of overloads for setting uniform values in the shaders
    // a template is not used here because each type requires a different OpenGL function call
    auto Set(const string &key, const bool  value) const -> void;
    auto Set(const string &key, const int   value) const -> void;
    auto Set(const string &key, const float value) const -> void;
    auto Set(const string &key, const vec2  value) const -> void;
    auto Set(const string &key, const vec3  value) const -> void;
    auto Set(const string &key, const vec4  value) const -> void;
    auto Set(const string &key, const mat2  value) const -> void;
    auto Set(const string &key, const mat3  value) const -> void;
    auto Set(const string &key, const mat4  value) const -> void;

    auto Set(const string &key, const Material &material) -> void;
};  
