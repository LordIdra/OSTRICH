#pragma once



auto ApplyDamping(float &value, float damping) -> void;
auto ApplySymmetricRange(float &value, float range) -> void;
auto ApplyAsymmetricRange(float &x, float min, float max) -> void;