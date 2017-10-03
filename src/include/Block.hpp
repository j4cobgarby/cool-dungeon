#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <SFML/Graphics.hpp>

#include "Box.hpp"

class Block {
public:
    Box box;
    sf::RectangleShape rect;

    Block(int x, int y, sf::Texture *tex);
};

#endif