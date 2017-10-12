#include "include/Player.hpp"

Weapon::Weapon() {}

Weapon::Weapon(string display_name, Texture *tex, unsigned short int damage, unsigned short int range) {
    this->display_name = display_name;

    this->rect.setPosition(Vector2f(0, 0));
    this->rect.setSize(Vector2f(45, 45));
    this->rect.setOrigin(Vector2f(rect.getSize().x/2, 0));
    this->rect.setTexture(tex);

    this->damage = damage;
    this->range = range;

    anim = Animation("swipe", 20);
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

    anim = Animation("player_idle_rt", 200);
}

void Player::update(Time *delta, Clock *g_clock, World *world, RenderWindow *window, const Vector2f *cursor_pos) {
    if (_score_timer.getElapsedTime().asMilliseconds() >= 4000) {
        _score_timer.restart();
        stats["sco"]++;
    }

    if (Keyboard::isKeyPressed(Keyboard::A) && _l) {
        box.vx -= delta->asSeconds() * _speed;
    } if (Keyboard::isKeyPressed(Keyboard::D) && _r) {
        box.vx += delta->asSeconds() * _speed;
    } if (Keyboard::isKeyPressed(Keyboard::W) && _u) {
        box.vy -= delta->asSeconds() * _speed;
    } if (Keyboard::isKeyPressed(Keyboard::S) && _d) {
        box.vy += delta->asSeconds() * _speed;
    }

    anim.set_key(facing == d_right ? "player_idle_rt" : "player_idle_lt");

    if (Keyboard::isKeyPressed(Keyboard::W)) {
        anim.set_key((facing == d_right ? "player_walk_uprt" : "player_walk_uplt"));
    }
    if (Keyboard::isKeyPressed(Keyboard::S)) {
        anim.set_key((facing == d_right ? "player_walk_rt" : "player_walk_lt"));
    }
    if (Keyboard::isKeyPressed(Keyboard::A)) {
        facing = d_left;
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            /** Up/left */
            anim.set_key("player_walk_uplt");
        } else {
            /** Left */
            anim.set_key("player_walk_lt");
        }
    } if (Keyboard::isKeyPressed(Keyboard::D)) {
        facing = d_right;
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            /** Up/right */
            anim.set_key("player_walk_uprt");
        } else {
            /** Right */
            anim.set_key("player_walk_rt");
        }
    }

    rect.setTexture(anim.get_frame(g_clock));
    weapon.update_texture(&_hit_timer);

    box.vx *= 0.9;
    box.vy *= 0.9;

    rect.setPosition(Vector2f(
        box.x - texture_offset_x,
        box.y - texture_offset_y
    ));

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

    Vector2f perceived_mouse_position = window->mapPixelToCoords((Vector2i)*cursor_pos);

    if (perceived_mouse_position.x > box.x + box.w/2)
        facing = d_right;
    else

    facing = d_left;

    float middle_x = box.x + box.w/2;
    float middle_y = box.y + box.h/2;

    Vector2f diff(middle_x - perceived_mouse_position.x, middle_y - perceived_mouse_position.y);
    diff = normalize(diff);

    weapon.rect.setRotation((atan2(diff.y, diff.x) * 180/PI) - 90);
    weapon.rect.setPosition(Vector2f(
        middle_x - diff.x*60,
        middle_y - diff.y*60
    ));
    if (!_hitting) weapon.rect.setFillColor(Color(0xffffff00));
    else weapon.rect.setFillColor(Color(0xffffffff));
    if (_hitting && _hit_timer.getElapsedTime().asSeconds() > 0.2) { // <- Visually the time taken to hit
        _hitting = false;
        cout << "Stopped hitting" << endl;
        weapon.rect.setFillColor(Color(0xffffff00));
    }

    if (_hitting && _hit_timer.getElapsedTime().asSeconds() > (0.2 / 2) && !_hit_already) {
        for (size_t b_index = 0; b_index < _baddies->size(); b_index++) {
            float angle_to_enemy = angleVecToVec(Vector2f(box.x, box.y), _baddies->at(b_index).rect.getPosition());
            float angle_to_mouse = angleVecToVec(Vector2f(box.x + box.w / 2, box.y + box.h / 2), perceived_mouse_position);

            float min_hit_angle = (weapon.rect.getRotation() - 45 - 90);
            float max_hit_angle = (weapon.rect.getRotation() + 45 - 90);

            cout << abs(angle_to_enemy) << "\t" << min_hit_angle << "\t" << max_hit_angle << endl;

            if (min_hit_angle <= abs(angle_to_enemy) && abs(angle_to_enemy) <= max_hit_angle &&
                vectorDist(Vector2f(box.x, box.y), _baddies->at(b_index).rect.getPosition()) <= 55)
            {
                _baddies->at(b_index).health -= weapon.damage;
                if (_baddies->at(b_index).health < 0) _baddies->at(b_index).health = 0;
            }
        }

        _hit_already = true;
    }
}

void Player::click(Time *delta, World *world, RenderWindow *window) {
    if (_hit_timer.getElapsedTime().asSeconds() > 0.2) { // <- The hit cooldown time
        _hitting = true;
        _hit_already = false;
        _hit_timer.restart();
    }
}
