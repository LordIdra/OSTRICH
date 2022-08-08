#pragma once

#include "../util/Types.h"

#include <iostream>



namespace logging {

    // https://gist.github.com/vicenteguerra/e81189c7242631cd0832ccbc6f1976f9
    const string WHITE = "\x1b[37m";
    const string YELLOW = "\x1b[33m";
    const string RED = "\x1b[31m";
    const string NO_COLOR = "\x1b[39m";

    auto Info(const string &message) -> void;
    auto Warning(const string &message) -> void;
    auto Error(const string &message) -> void;
}


