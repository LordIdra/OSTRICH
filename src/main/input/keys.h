#pragma once



namespace keys {
    auto Init() -> void;
    auto BindFunctionToKeyPress(int key, void (*function)()) -> void;
    auto BindFunctionToKeyRelease(int key, void (*function)()) -> void;
    auto KeyHeldDown(int key) -> bool;
}