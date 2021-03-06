#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <fstream>
#include <iostream>

#include "Block.hpp"
#include "asset_registers.hpp"

class World {
public:
    World(std::ifstream *file);

    std::vector<Block> collisions;
    std::vector<Block> background;

    sf::Vector2f spawn_position;
    sf::Vector2f exit_position;
};

#endif
