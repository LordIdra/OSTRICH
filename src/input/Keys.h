#pragma once



namespace Keys {
    auto Init() -> void;
    auto Update() -> void;

    auto KeyHeldDown(int key) -> bool;

    auto BindFunctionToKeyPress(const int key, void (*function)()) -> void;
    auto BindFunctionToKeyRelease(const int key, void (*function)()) -> void;
    auto BindFunctionToKeyHold(const int key, void (*function)()) -> void;
}