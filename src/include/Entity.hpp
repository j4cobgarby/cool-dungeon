#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

#include "Box.hpp"
#include "World.hpp"
#include "asset_registers.hpp"

enum Direction {d_left, d_right};

class Entity {
public:
    Box box;
    sf::RectangleShape rect;
    float texture_offset_x, texture_offset_y;

    Entity();
    Entity(const float x, const float y,
        const float box_w, const float box_h,
        const float rect_w, const float rect_h,
        const float texture_offset_x, const float texture_offset_y,
        const float vx, const float vy,
        sf::Texture *tex);
};

class Baddie : public Entity {
private:
    bool
    _u=true,
    _d=true,
    _l=true,
    _r=true;

    bool _collides;
    float _speed;
    Entity *_following;
    string _animation_key;
public:
    Baddie() {}
    Baddie(Vector2f position, const unsigned int health, const unsigned int attack, const unsigned int defense, Entity *following, bool collides, float speed, string animation_key);

    int health;
    int attack;
    int defense;

    void update(Time *delta, Clock *g_clock, World *world, RenderWindow *window, vector<Baddie> *baddies);
};

#endif
