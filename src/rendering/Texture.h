#pragma once

#include <util/Types.h>


class Texture {
private:
    int width;
    int height;
    int channels;
    unsigned int id;

public:
    Texture(const char *path);

    auto Bind() const -> void;
};