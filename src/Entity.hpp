#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

#include "Box.hpp"
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

#endif