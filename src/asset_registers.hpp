#ifndef ASSET_REGISTERS_HPP
#define ASSET_REGISTERS_HPP

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

inline Texture makeTexture(string filename) {
    Texture tex;
    tex.loadFromFile(filename);
    return tex;
}

inline Texture makeTexture(string filename, IntRect area) {
    Texture tex;
    tex.loadFromFile(filename, area);
    return tex;
}

inline Font makeFont(string filename) {
    Font fnt;
    fnt.loadFromFile(filename);
    return fnt;
}

extern map<int, Texture>    tilemap_register;
extern map<string, Texture> texture_register;
extern map<string, Font> font_register;

#endif