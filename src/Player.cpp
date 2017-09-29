#include "Player.hpp"

Weapon::Weapon() {}

Weapon::Weapon(string display_name, Texture *tex, unsigned short int damage, unsigned short int range) {
    this->display_name = display_name;

    this->rect.setPosition(Vector2f(0, 0));
    this->rect.setSize(Vector2f(30, 30));
    this->rect.setOrigin(Vector2f(0, 30)); // Bottom left origin
    this->rect.setTexture(tex);
    this->rect.setRotation(-45); // Pointing up

    this->damage = damage;
    this->range = range;
}

Player::Player(Vector2f position, Weapon weapon) :
    Entity(position.x, position.y,
        9*1.875, 15*1.875,
        30, 30,
        1*1.875, 0,
        0, 0,
        &texture_register["player"])
        {

    this->weapon = weapon;
}

void Player::update(Time *delta, World *world, RenderWindow *window, const Vector2f *cursor_pos) {
    Vector2i mouse_position = Mouse::getPosition(*window);

    box.vx *= 0.9;
    box.vy *= 0.9;

    if (Keyboard::isKeyPressed(Keyboard::A) && l) {
        box.vx -= delta->asSeconds() * SPEED;
    } if (Keyboard::isKeyPressed(Keyboard::D) && r) {
        box.vx += delta->asSeconds() * SPEED;
    } if (Keyboard::isKeyPressed(Keyboard::W) && u) {
        box.vy -= delta->asSeconds() * SPEED;
    } if (Keyboard::isKeyPressed(Keyboard::S) && d) {
        box.vy += delta->asSeconds() * SPEED;
    }

    rect.setPosition(Vector2f(box.x - texture_offset_x, box.y - texture_offset_y));

    /**
     * Collisions
     */

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
            d = false;
            u = true;
        } else if (normaly == 1) {
            d = true;
            u = false;
        }
        if (normalx == -1) {
            l = true;
            r = false;
        } else if (normalx == 1) {
            l = false;
            r = true;
        }
    } else {
        u = true;
        d = true;
        l = true;
        r = true;
    }

    /**
     * Visual aspect of the sword
     */

    Vector2f perceived_mouse_position = window->mapPixelToCoords((Vector2i)*cursor_pos);

    float middle_x = box.x + box.w/2;
    float middle_y = box.y + box.h/2;

    Vector2f diff(middle_x - perceived_mouse_position.x, middle_y - perceived_mouse_position.y);
    diff = normalize(diff);

    weapon.rect.setRotation(lerpangle(
        weapon.rect.getRotation(),
        (atan2(diff.y, diff.x) * 180/PI) + 225,
        delta->asSeconds() * 20
    ));
    weapon.rect.setPosition(Vector2f(
        middle_x - diff.x*20,
        middle_y - diff.y*20
    ));
}

void Player::click(Time *delta, World *world, RenderWindow *window) {
    cout << "Clicked" << endl;

    Vector2f perceived_mouse_position = window->mapPixelToCoords(Mouse::getPosition(*window));
    
    float middle_x = box.x + box.w/2;
    float middle_y = box.y + box.h/2;

    Vector2f diff(middle_x - perceived_mouse_position.x, middle_y - perceived_mouse_position.y);
    diff = normalize(diff);

    weapon.rect.setRotation(lerpangle(
        weapon.rect.getRotation(),
        (atan2(diff.y, diff.x) * 180/PI) + 225,
        delta->asSeconds() * 20
    ));
    weapon.rect.setPosition(Vector2f(
        middle_x - diff.x*20,
        middle_y - diff.y*20
    ));
}
