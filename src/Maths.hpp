#ifndef MATHS_HPP
#define MATHS_HPP

inline float lerp(float from, float to, float progress) {
    return from + (to - from) * progress;
}

inline float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

#endif