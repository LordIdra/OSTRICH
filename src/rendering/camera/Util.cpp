#include "Util.h"

#include <util/Constants.h>



auto ApplyDamping(float &value, float damping) -> void {
    // A small value is added to the threshold for floating point errors
    damping += FLOATING_POINT_ADJUSTMENT;

    // Damp the value by pushing it towards zero
    // If it's sufficiently close so zero, clamp it to zero
    // If we didn't do this, the value would never quite reach zero
    if (value < -damping) {
        value += damping;
    } else if (value > damping) {
        value -= damping;
    } else {
        value = 0;
    }
}

auto ApplySymmetricRange(float &value, float range) -> void {
    // A small value is added to the range for floating point errors
    range += FLOATING_POINT_ADJUSTMENT;

    // Clamp the value between range and -range
    if (value < -range) {
        value = -range;
    } else if (value > range) {
        value = range;
    }
}

auto ApplyAsymmetricRange(float &x, float min, float max) -> void {
    // A small value is added to the min and max for floating point errors
    if (x < min) {
        x = min;
    } else if (x > max) {
        x = max;
    }
}