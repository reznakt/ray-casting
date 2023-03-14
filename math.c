#include <stdbool.h>
#include <math.h>


#include "math.h"


float map(const float value, const float old_low, const float old_high,
          const float new_low, const float new_high) {
    return new_low + (value * (old_high - old_low) * (new_high - new_low));
}

float constrain(const float value, const float low, const float high) {
    if (value < low) {
        return low;
    }

    if (value > high) {
        return high;
    }

    return value;
}

bool isclose(const float a, const float b, const float epsilon) {
    return fabsf(a - b) < epsilon;
}

float degrees(float angle) {
    return angle * 180.0f / (float) M_PI;
}

float radians(float angle) {
    return angle * (float) M_PI / 180.0f;
}

