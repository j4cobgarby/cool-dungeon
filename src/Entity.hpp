#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

#include "Box.hpp"

class Entity {
public:
    Box box;
    sf::RectangleShape rect;

    Entity(float x, float y, float w, float h, float vx, float vy, sf::Texture *tex);
    Entity(float x, float y, float w, float h, sf::Texture *tex);
};

#endif