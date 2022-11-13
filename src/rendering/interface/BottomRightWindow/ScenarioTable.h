#pragma once

#include <util/Types.h>

#include <imgui.h>



namespace ScenarioTable {
    const ImVec2 TABLE_SIZE = ImVec2(600, 400);

    auto AddFileTable() -> void;

    auto GetSelectedFile() -> string;
}