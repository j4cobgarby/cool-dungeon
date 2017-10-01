#include "Baddie.hpp"

Baddie::Baddie(Vector2f position, 
    const unsigned int health, const unsigned int attack, const unsigned int defense, 
    Entity *following) : Entity(
    position.x, position.y, 30, 30, 30, 30, 0, 0, 0, 0, NULL
) {
    this->health = health;
    this->attack = attack;
    this->defense = defense;

    this->_following = following;
}

void Baddie::update(Time *delta, Clock *g_clock, World *world, RenderWindow *window) {
    /**
     * Basically just move towards the player.
     */

     rect.setTexture(&animation_register["ghost"].at(
         (g_clock->getElapsedTime().asMilliseconds() / 300) % animation_register["ghost"].size()
     ));

    // Calculate the delta x and y from the baddie to the entity the baddie's following
    float dx, dy;
    dx = _following->box.x - box.x;
    dy = _following->box.y - box.y;

    // normalise the delta x and y as a vector
    Vector2f direction = normalize(dx, dy);         //

    // apply velocity along this vector
    if ((direction.x < 0 && _l) || (direction.x > 0 && _r)) box.vx += direction.x * delta->asSeconds() * 10;
    if ((direction.y < 0 && _u) || (direction.y > 0 && _d)) box.vy += direction.y * delta->asSeconds() * 10;
    box.vx *= 0.9;
    box.vy *= 0.9;

    rect.setPosition(Vector2f(
        box.x - texture_offset_x,
        box.y - texture_offset_y
    ));

    // Check for collisions

    Box broadphasebox = GetSweptBroadphaseBox(box);

    float normalx, normaly;
    float collisiontime = 1;

    for (Block block : world->collisions) {
        Box b = block.box;
        float bnormalx, bnormaly, bcollisiontime;
        bcollisiontime = SweptAABB(box, b, &bnormalx, &bnormaly);
        if (bcollisiontime != 1 && bcollisiontime < 1 && bcollisiontime < collisiontime && AABBCheck(broadphasebox, b)) {
            collisiontime = bcollisiontime;
            normalx = bnormalx;
            normaly = bnormaly;
        }
    }

    box.x += box.vx * collisiontime;
    box.y += box.vy * collisiontime;
    float remainingtime = 1.0f - collisiontime;

    if (remainingtime != 0) {
        float dotprod = (box.vx * normaly + box.vy * normalx) * remainingtime;
        box.vx = dotprod * normaly;
        box.vy = dotprod * normalx;
        box.vx *= 1.12;
        box.vy *= 1.12;

        if (normaly == -1) {
            _d = false;
            _u = true;
        } else if (normaly == 1) {
            _d = true;
            _u = false;
        }
        if (normalx == -1) {
            _l = true;
            _r = false;
        } else if (normalx == 1) {
            _l = false;
            _r = true;
        }
    } else {
        _u = true;
        _d = true;
        _l = true;
        _r = true;
    }
}