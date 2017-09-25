#ifndef TEX_REGISTER_HPP
#define TEX_REGISTER_HPP

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

extern map<int, Texture> tilemap_register;
extern map<string, Texture> texture_register;

#endif