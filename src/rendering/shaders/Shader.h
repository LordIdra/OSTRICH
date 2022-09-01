#pragma once

#include <util/Types.h>



class Shader {
private:
    unsigned int id;

    static auto FileExists(const string &path) -> bool;
    static auto ReadFile(const string &path) -> string;
    
    auto CheckCompileSuccess(const string &path) const -> void;
    auto PrintCompileLog() const -> void;

public:
    Shader();
    
    // the constructor will compile the source into a shader managed by OpenGL
    auto Init(const string &path, const unsigned int type) -> void;

    // attach the shader to a program
    auto Attach(const unsigned int program) const -> void;

    // after being linked into a program, the shaders are not needed any more, so this destructor can be called
    // the reason a custom destructor is needed is to delete the internal shaders managed by OpenGL; not doing so could be considered a memory leak
    ~Shader();
};
