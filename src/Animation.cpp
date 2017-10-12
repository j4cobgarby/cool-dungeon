#include "include/Animation.hpp"

Animation::Animation(string key, int time_divisor) {
    this->_key = key;
    this->_time_divisor = time_divisor;
}

Animation::Animation() {}

Texture *Animation::get_frame(Clock *t) {
    return &animation_register[_key].at((t->getElapsedTime().asMilliseconds() / _time_divisor) % animation_register[_key].size());
}