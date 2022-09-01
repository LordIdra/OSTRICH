#include "Shader.h"

#include "../util/Log.h"

#include <glad/glad.h>

#include <sstream>
#include <fstream>

using std::ifstream;
using std::stringstream;



Shader::Shader() : id(0) {}

Shader::~Shader() {
    glDeleteShader(id);
}

auto Shader::FileExists(const string &path) -> bool {
    // Open a file as an ifstream
    ifstream file(path);
    
    // Check if the opened file contains any data - if so, the file exists
    return bool(file);
}

auto Shader::ReadFile(const string &path) -> string {
    // Check the file actually exists
    if (!FileExists(path)) {
        Log::Error("Failed to open file at " + path);
    }

    // Open the file and read it into a stringstream
    ifstream file(path);
    stringstream sourceBuffer;
    sourceBuffer << file.rdbuf();
    file.close();
    return sourceBuffer.str();
}

auto Shader::PrintCompileLog() const -> void {
    // Create a log array
    const unsigned int LOG_LENGTH = 1024;
    std::array<char, LOG_LENGTH> compileLog {};

    // Read the info log into the log array, then display the log
    glGetShaderInfoLog(id, LOG_LENGTH, nullptr, (char*)compileLog.begin());
    Log::Error((string) compileLog.begin());
}

auto Shader::CheckCompileSuccess(const string &path) const -> void {
    // Read the compile success state into a variable
    int compileSuccessful = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compileSuccessful);   

    // Log a message depending on whether the compile was successful
    if (!compileSuccessful) {
        Log::Error("Failed to compile shader at " + path);
        PrintCompileLog();
        return;
    }
}

auto Shader::Init(const string &path, const unsigned int type) -> void {
    // Create a shader and read the the file at 'path' into a string
    id = glCreateShader(type);
    const string stringSource = ReadFile(path);

    // Convert the string to a char array and set the shader source
    const char *source = stringSource.c_str();
    glShaderSource(id, 1, &source, nullptr);

    // Attempt to compile the shader and check that it actually compiled
    glCompileShader(id);
    CheckCompileSuccess(path);
}

auto Shader::Attach(const unsigned int program) const -> void {
    glAttachShader(program, id);
}
