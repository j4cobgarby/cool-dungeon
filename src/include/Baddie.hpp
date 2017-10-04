#ifndef BADDIE_HPP
#define BADDIE_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "asset_registers.hpp"
#include "AABB.hpp"

using namespace std;
using namespace sf;

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
    Baddie(Vector2f position, const unsigned int health, const unsigned int attack, const unsigned int defense, Entity *following, bool collides, float speed, string animation_key);

    unsigned int health;
    unsigned int attack;
    unsigned int defense;

    void update(Time *delta, Clock *g_clock, World *world, RenderWindow *window, vector<Baddie> *baddies);
};

#endif
