#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <string>

#include "asset_registers.hpp"

using namespace std;
using namespace sf;

class Animation {
private:
    string _key;
    int _time_divisor;
public:
    Animation();
    Animation(string key, int time_multiplier);

    void set_key(string key) {_key = key;}

    Texture *get_frame(Clock *time);
};

#endif