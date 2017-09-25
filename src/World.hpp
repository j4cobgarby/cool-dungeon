#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <fstream>
#include <iostream>

#include "Block.hpp"
#include "Entity.hpp"
#include "tex_register.hpp"

class World {
public:
    World(std::ifstream *file);

    std::vector<Block> collisions;
    std::vector<Block> background;
};

#endif