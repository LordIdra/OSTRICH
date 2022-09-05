#pragma once

#include <util/Types.h>



class Shader {
private:
    unsigned int id;


public:
    Shader(const string &path, const unsigned int type);
    ~Shader();
    
    auto Attach(const unsigned int program) const -> void;
};
