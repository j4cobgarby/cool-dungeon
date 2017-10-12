#ifndef MATHS_HPP
#define MATHS_HPP

#define PI 3.1415926

#include <SFML/Graphics.hpp>
#include <cmath>

inline float lerp(const float from, const float to, const float progress) {
    return from + (to - from) * progress;
}

/**
 * In degrees.
 */
inline float lerpangle(const float from, const float to, const float progress) {
    float delta = fmod(to - from + 360 + 180, 360) - 180;
    return fmod(from + delta * progress + 360, 360);
}

inline float clamp(const float value, const float min, const float max) {
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

inline Vector2f normalize(float x, float y) {
    return normalize(Vector2f(x, y));
}

inline float angleVecToVec(Vector2f v1, Vector2f v2) {
    return (float)atan2(v2.y - v1.y, v2.x - v1.x) * (180/PI);
}

inline float vectorDist(Vector2f v1, Vector2f v2) {
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2));
}

#endif