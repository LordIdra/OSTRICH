#pragma once

#include <util/Types.h>




namespace Scenarios {
    auto SaveScenario(const string &path) -> void;
    auto LoadScenario(const string &path) -> void;
}