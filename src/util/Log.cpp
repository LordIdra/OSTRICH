#include "Log.h"

#include <iostream>
#include <unordered_map>

using std::cout;
using std::unordered_map;


namespace {
    // https://gist.github.com/vicenteguerra/e81189c7242631cd0832ccbc6f1976f9
    const string GREEN = "\x1b[32m";
    const string GRAY = "\x1b[90m";
    const string WHITE = "\x1b[37m";
    const string YELLOW = "\x1b[33m";
    const string RED = "\x1b[31m";
    const string NO_COLOR = "\x1b[39m";
    
    unordered_map<Level, string> prefixes {
        {SUCCESS, GREEN + "[SUCCESS] " + NO_COLOR},
        {DEBUG, GRAY + "[DEBUG] " + NO_COLOR},
        {INFO, WHITE + "[INFO] " + NO_COLOR},
        {WARN, YELLOW + "[WARN] " + NO_COLOR},
        {ERROR, RED + "[ERROR] " + NO_COLOR}
    };
}

auto Log(const Level level, const string &message) -> void {
    cout << prefixes.at(level) << message << "\n";
}
