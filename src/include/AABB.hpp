#ifndef AABB_HPP
#define AABB_HPP

#include "Box.hpp"

#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;

extern float SweptAABB(Box b1, Box b2, float *normalx, float *normaly);
extern Box GetSweptBroadphaseBox(Box b);
extern bool AABBCheck(Box b1, Box b2);

#endif