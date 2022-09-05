#include "Shader.h"

#include <rendering/shaders/Util.h>

#include <glad/glad.h>



Shader::~Shader() {
    glDeleteShader(id);
}

Shader::Shader(const string &path, const unsigned int type){
    // Create a shader and read the the file at 'path' into a string
    id = glCreateShader(type);
    const string stringSource = ReadFile(path);

    // Convert the string to a char array and set the shader source
    const char *source = stringSource.c_str();
    glShaderSource(id, 1, &source, nullptr);

    // Attempt to compile the shader and check that it actually compiled
    glCompileShader(id);
    CheckCompileSuccess(path, id);
}

auto Shader::Attach(const unsigned int program) const -> void {
    glAttachShader(program, id);
}
