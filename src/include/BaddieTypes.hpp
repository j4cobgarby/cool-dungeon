#ifndef BADDIETYPES_HPP
#define BADDIETYPES_HPP

#include "Baddie.hpp"

class Ghost : public Baddie {
public:
  Ghost(Vector2f position, Entity *following) : Baddie(position, 5, 2, 1, following, false, 5, "ghost") {}
};

#endif
