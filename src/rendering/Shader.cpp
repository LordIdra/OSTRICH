#include "Shader.h"

#include "../util/Log.h"

#include <glad/glad.h>

#include <sstream>
#include <fstream>

using std::ifstream;
using std::stringstream;



auto Shader::FileExists(const string &path) const -> bool {
    ifstream file(path);
    return bool(file);
}

auto Shader::ReadFile(const string &path) const -> string {
    if (!FileExists(path)) {
        Log::Error("Failed to open file at " + path);
    }
    ifstream file(path);
    stringstream sourceBuffer;
    sourceBuffer << file.rdbuf();
    file.close();
    return sourceBuffer.str();
}

auto Shader::PrintCompileLog() const -> void {
    const unsigned int LOG_LENGTH = 1024;
    char compileLog[LOG_LENGTH];
    glGetShaderInfoLog(id, LOG_LENGTH, nullptr, compileLog);
    Log::Error((string) compileLog);
}

auto Shader::CheckCompileSuccess(const string &path) const -> void {
    int compileSuccessful = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compileSuccessful);   
    if (!compileSuccessful) {
        Log::Error("Failed to compile shader at " + path);
        PrintCompileLog();
    } else {
        Log::Info("Successfully compiled shader at " + path);
    }
}

Shader::Shader() {}

auto Shader::Init(const string &path, const unsigned int type) -> void {
    id = glCreateShader(type);
    const string stringSource = ReadFile(path);
    const char *source = stringSource.c_str();
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);
    CheckCompileSuccess(path);
}

auto Shader::Attach(const unsigned int program) const -> void {
    glAttachShader(program, id);
}

Shader::~Shader() {
    glDeleteShader(id);
}
