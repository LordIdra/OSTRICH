#pragma once

#include <util/Types.h>



class Shader {
private:
    unsigned int id;


public:
    Shader();
    ~Shader();
    
    auto Init(const string &path, const unsigned int type) -> void;
    auto Attach(const unsigned int program) const -> void;
};
