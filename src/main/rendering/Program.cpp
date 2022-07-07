#include "Program.h"

#include "../util/logging.h"

#include <glad/glad.h>



auto Program::CheckLinkSuccess() const -> void {
    int linkSuccessful = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &linkSuccessful);
    if (!linkSuccessful) {
        logging::Error("Failed to link program");
        PrintLinkLog();
    } else {
        logging::Info("Successfully linked program");
    }
}

auto Program::PrintLinkLog() const -> void {
    const unsigned int LOG_LENGTH = 1024;
    char linkLog[LOG_LENGTH];
    glGetProgramInfoLog(id, LOG_LENGTH, nullptr, linkLog);
    logging::Error((string)linkLog);
}

Program::Program() {}

auto Program::Init() -> void{
    id = glCreateProgram();
}

auto Program::AddShader(const Shader &shader) const -> void {
    shader.Attach(id);
}

auto Program::Link() -> void {
    glLinkProgram(id);
    CheckLinkSuccess();
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

auto Program::Set(const string &key, const Material &material) -> void {
    Set(key + ".ambient", material.ambient);
    Set(key + ".diffuse", material.diffuse);
    Set(key + ".specular", material.specular);
    Set(key + ".shine", material.shine);
}



