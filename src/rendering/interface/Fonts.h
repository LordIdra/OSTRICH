#pragma once

#include <imgui.h>

#include <util/Types.h>
#include <depend/IconsMaterialDesignIcons_c.h>



namespace Fonts {
    auto Init() -> void;
    auto NormalizeString(ImFont* font, const string &str, const float maxWidth) -> string;

    auto Main() -> ImFont*;
    auto MainBig() -> ImFont*;
    auto Data() -> ImFont*;
}