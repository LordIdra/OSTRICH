#include "Program.h"

#include <rendering/shaders/Util.h>

#include <glad/glad.h>



Program::Program() : id(0) {}

Program::~Program() {
    glDeleteProgram(id);
}

auto Program::Init() -> void {
    id = glCreateProgram();
}

auto Program::AddShader(const Shader &shader) const -> void {
    shader.Attach(id);
}

auto Program::Link() const -> void {
    glLinkProgram(id);
    CheckLinkSuccess(id);
}

auto Program::Use() const -> void {
    glUseProgram(id);
}

auto Program::Set(const string &key, const bool  value) const -> void {
    glUniform1i(glGetUniformLocation(id, key.c_str()), (int)value); 
}

auto Program::Set(const string &key, const int   value) const -> void {
    glUniform1i(glGetUniformLocation(id, key.c_str()), value); 
}

auto Program::Set(const string &key, const float value) const -> void {
    glUniform1f(glGetUniformLocation(id, key.c_str()), value); 
}

auto Program::Set(const string &key, const vec2  value) const -> void {
    glUniform2fv(glGetUniformLocation(id, key.c_str()), 1, &value[0]);
}

auto Program::Set(const string &key, const vec3  value) const -> void {
    glUniform3fv(glGetUniformLocation(id, key.c_str()), 1, &value[0]);
}

auto Program::Set(const string &key, const vec4  value) const -> void {
    glUniform4fv(glGetUniformLocation(id, key.c_str()), 1, &value[0]);
}

auto Program::Set(const string &key, const mat2  value) const -> void {
    glUniformMatrix2fv(glGetUniformLocation(id, key.c_str()), 1, false, &value[0][0]);
}

auto Program::Set(const string &key, const mat3  value) const -> void {
    glUniformMatrix3fv(glGetUniformLocation(id, key.c_str()), 1, false, &value[0][0]);
}

auto Program::Set(const string &key, const mat4  value) const -> void {
    glUniformMatrix4fv(glGetUniformLocation(id, key.c_str()), 1, false, &value[0][0]);
}

auto Program::Set(const string &key, const Material &material) const -> void {
    Set(key + ".ambient", material.ambient);
    Set(key + ".diffuse", material.diffuse);
    Set(key + ".specular", material.specular);
    Set(key + ".shine", material.shine);
}
