#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <thread>

#include "Entity.hpp"
#include "asset_registers.hpp"
#include "AABB.hpp"
#include "World.hpp"
#include "Maths.hpp"
#include "Animation.hpp"

using namespace std;
using namespace sf;

class Weapon {
private:
    Animation anim;

public:
    Weapon();
    Weapon(string display_name, Texture *tex, unsigned short int damage, unsigned short int range);

    string display_name;
    RectangleShape rect;

    /** Stats */
    unsigned short int damage;
    unsigned short int range;

    void update_texture(Clock *g_clock) {rect.setTexture(anim.get_frame(g_clock));}
};

class Player : public Entity {
private:
    bool _u = true, _d = true, _l = true, _r = true;
    const unsigned short int _speed = 20;

    Direction facing = d_right;
    Clock _score_timer;
    Clock _hit_timer;
    Animation anim;
    vector<Baddie> *_baddies;
public:
    bool _hitting = false;
    Player(Vector2f position, Weapon weapon);
    Weapon weapon;

    map<string, short int> stats {
        {"hea", 10},
        {"atk", 0},
        {"def", 0},
        {"dex", 0},
        {"sco", 0},
    };

    void set_baddies(vector<Baddie> *baddies) {_baddies = baddies;}
    void update(Time *delta, Clock *g_clock, World *world, RenderWindow *window, const Vector2f *cursor_pos);
    void click(Time *delta, World *world, RenderWindow *window);
};

#endif