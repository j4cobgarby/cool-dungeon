#include "include/Block.hpp"

using namespace sf;

Block::Block(int x, int y, Texture *tex) {
    box.x = x;
    box.y = y;
    box.w = 50;
    box.h = 50;

    rect.setPosition(Vector2f(x, y));
    rect.setSize(Vector2f(50, 50));
    rect.setTexture(tex);
}
