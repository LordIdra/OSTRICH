#pragma once

#include <string>

using std::string;



enum Level {
    SUCCESS,
    DEBUG,
    INFO,
    WARN,
    ERROR
};

auto Log(const Level level, const string &message) -> void;
