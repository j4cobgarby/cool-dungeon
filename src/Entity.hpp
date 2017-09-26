#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

#include "Box.hpp"
#include "tex_register.hpp"

class Entity {
public:
    Box box;
    sf::RectangleShape rect;
    float texture_offset_x, texture_offset_y;

    Entity();
    Entity(float x, float y, 
        float box_w, float box_h, 
        float rect_w, float rect_h, 
        float texture_offset_x, float texture_offset_y, 
        float vx, float vy, 
        sf::Texture *tex);
};

#endif