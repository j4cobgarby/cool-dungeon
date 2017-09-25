#include "Entity.hpp"

using namespace sf;

Entity::Entity(float x, float y, float w, float h, float vx, float vy, Texture *tex) {
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;
    box.vx = vx;
    box.vy = vy;

    rect.setPosition(Vector2f(x, y));
    rect.setSize(Vector2f(w, h));
    rect.setTexture(tex);
    rect.setFillColor(Color::Transparent);
    rect.setOutlineColor(Color::Green);
    rect.setOutlineThickness(1);
}