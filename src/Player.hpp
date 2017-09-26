#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <string>

#include "Entity.hpp"
#include "tex_register.hpp"
#include "AABB.hpp"
#include "World.hpp"
#include "Maths.hpp"

#define SPEED 30

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
public:
    Player(Vector2f position, Weapon weapon);

    Weapon weapon;

    unsigned short int health; /** As a percentage (0-100) */

    bool u=true,d=true,l=true,r=true;

    map<string, unsigned short int> stats {
        {"atk", 0},
        {"def", 0},
        {"dex", 0},
    };

    void update(Time *delta, World *world, RenderWindow *window) ;
};

#endif