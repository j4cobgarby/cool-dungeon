#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Entity.hpp"
#include "asset_registers.hpp"
#include "AABB.hpp"
#include "World.hpp"
#include "Maths.hpp"

#define SPEED 30

using namespace std;
using namespace sf;

class Weapon {
public:
    Weapon();
    Weapon(string display_name, Texture *tex, unsigned short int damage, unsigned short int range);

    string display_name;
    RectangleShape rect;

    /** Stats */
    unsigned short int damage;
    unsigned short int range;
};

class Player : public Entity {
private:
    bool _u=true,_d=true,_l=true,_r=true;

    Direction facing = d_right;
    Clock _score_timer;
public:
    Player(Vector2f position, Weapon weapon);

    Weapon weapon;

    map<string, short int> stats {
        {"hea", 10},
        {"atk", 0},
        {"def", 0},
        {"dex", 0},
        {"sco", 0},
    };

    void update(Time *delta, Clock *g_clock, World *world, RenderWindow *window, const Vector2f *cursor_pos);
    void click(Time *delta, World *world, RenderWindow *window);
};

#endif