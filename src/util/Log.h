#pragma once

#include <util/Types.h>



enum Level {
    SUCCESS,
    DEBUG,
    INFO,
    WARN,
    ERROR
};

auto Log(const Level level, const string &message) -> void;
