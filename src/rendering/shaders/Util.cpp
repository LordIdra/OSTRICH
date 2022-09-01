#include "Util.h"

#include <util/Log.h>

#include <sstream>
#include <fstream>

#include <glad/glad.h>

using std::ifstream;
using std::stringstream;



auto FileExists(const string &path) -> bool {
    // Open a file as an ifstream
    ifstream file(path);
    
    // Check if the opened file contains any data - if so, the file exists
    return bool(file);
}

auto ReadFile(const string &path) -> string {
    // Check the file actually exists
    if (!FileExists(path)) {
        Log(ERROR, "Failed to open file at " + path);
    }

    // Open the file and read it into a stringstream
    ifstream file(path);
    stringstream sourceBuffer;
    sourceBuffer << file.rdbuf();
    file.close();
    return sourceBuffer.str();
}

auto PrintCompileLog(const unsigned int id) -> void {
    // Create a log array
    const unsigned int LOG_LENGTH = 1024;
    std::array<char, LOG_LENGTH> log {};

    // Read the info log into the log array, then display the log
    glGetShaderInfoLog(id, LOG_LENGTH, nullptr, (char*)log.begin());
    Log(ERROR, string(log.begin()));
}

auto PrintLinkLog(const unsigned int id) -> void {
    // Create a log array
    const unsigned int LOG_LENGTH = 1024;
    std::array<char, LOG_LENGTH> log {};

    // Read the info log into the log array, then display the log
    glGetProgramInfoLog(id, LOG_LENGTH, nullptr, (char*)log.begin());
    Log(ERROR, string(log.begin()));
}

auto CheckCompileSuccess(const string &path, const unsigned int id) -> void {
    // Read the compile success state into a variable
    int success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);   

    // Log a message depending on whether the compile was successful
    if (!success) {
        Log(ERROR, "Failed to compile shader at " + path);
        PrintCompileLog(id);
    }
}

auto CheckLinkSuccess(const unsigned int id) -> void {
    // Read the link success state into a variable
    int success = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    // Log a message depending on whether the link was successful
    if (!success) {
        Log(ERROR, "Failed to link program");
        PrintLinkLog(id);
    }
}