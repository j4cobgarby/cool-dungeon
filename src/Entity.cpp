#include "Entity.hpp"

using namespace sf;

Entity::Entity() {
}

Entity::Entity(float x, float y, float box_w, float box_h, float rect_w, float rect_h, float texture_offset_x, float texture_offset_y, float vx, float vy, sf::Texture *tex) {
    this->texture_offset_x = texture_offset_x;
    this->texture_offset_y = texture_offset_y;

    box.x = x + texture_offset_x;
    box.y = y + texture_offset_y;
    box.w = box_w;
    box.h = box_h;
    box.vx = vx;
    box.vy = vy;

    rect.setPosition(Vector2f(x, y));
    rect.setSize(Vector2f(rect_w, rect_h));
    rect.setTexture(tex);
}