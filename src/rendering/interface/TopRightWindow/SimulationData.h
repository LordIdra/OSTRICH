#pragma once



namespace SimulationData {
    auto PreReset() -> void;
    auto PostReset() -> void;
    auto Draw(const double deltaTime) -> void;
}