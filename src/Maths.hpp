#ifndef MATHS_HPP
#define MATHS_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

inline float lerp(float from, float to, float progress) {
    return from + (to - from) * progress;
}

/**
 * In degrees.
 */
inline float lerpangle(float from, float to, float progress) {
    float delta = fmod(to - from + 360 + 180, 360) - 180;
    return fmod(from + delta * progress + 360, 360);
}

inline float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

inline Vector2f normalize(Vector2f vec) {
    float length = sqrt(pow(vec.x, 2) + pow(vec.y, 2));

    if (length != 0) { // Avoid division by zero
        vec.x /= length;
        vec.y /= length;
    }

    return vec;
}

#endif