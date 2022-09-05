#pragma once

#include <util/Types.h>
#include <rendering/structures/Material.h>
#include <rendering/shaders/Shader.h>



class Program {
private:
    unsigned int id;


public:
    Program(const Shader &vertex, const Shader &fragment);
    ~Program();

    auto Use() const -> void;

    auto Set(const string &key, const bool  value) const -> void;
    auto Set(const string &key, const int   value) const -> void;
    auto Set(const string &key, const float value) const -> void;
    auto Set(const string &key, const vec2  value) const -> void;
    auto Set(const string &key, const vec3  value) const -> void;
    auto Set(const string &key, const vec4  value) const -> void;
    auto Set(const string &key, const mat2  value) const -> void;
    auto Set(const string &key, const mat3  value) const -> void;
    auto Set(const string &key, const mat4  value) const -> void;
    auto Set(const string &key, const Material &material) const -> void;
};  
