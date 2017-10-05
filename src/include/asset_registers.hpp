#ifndef ASSET_REGISTERS_HPP
#define ASSET_REGISTERS_HPP

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

inline Texture makeTexture(const string filename) {
    Texture tex;
    tex.loadFromFile(filename);
    return tex;
}

inline Texture makeTexture(const string filename, const IntRect area) {
    Texture tex;
    tex.loadFromFile(filename, area);
    return tex;
}

inline Font makeFont(const string filename) {
    Font fnt;
    fnt.loadFromFile(filename);
    return fnt;
}

/**
 * Returns a vector of Textures to describe an animation.
 *
 * The file which `filename` refers to must contain `frames` frames, all
 * `frame_width`px wide and `frame_height`px tall, in a straight horizontal
 * line with no offset and no padding.
 */
inline vector<Texture> makeAnimation(const string filename, const unsigned short int frames, const unsigned short frame_width, const unsigned short frame_height) {
    vector<Texture> ret;

    for (int _frame_index = 0; _frame_index < frames; _frame_index++) {
        Texture _tmp;
        _tmp.loadFromFile(filename, IntRect(_frame_index * frame_width, 0, frame_width, frame_height));
        ret.push_back(_tmp);
    }
    return ret;
}

inline map<int, Texture> init_tilemap_register(const string filename,
        const unsigned int tiles_x, const unsigned int tiles_y, // Amount of tiles on each axis
        const unsigned int tile_width, const unsigned int tile_height) { // Width in px of each tile texture
    map<int, Texture> reg;
    unsigned short int index = 1;
    for (unsigned int y = 0; y < tiles_y; y++) {
        for (unsigned int x = 0; x < tiles_x; x++) {
            reg[index] = makeTexture(filename, IntRect(tile_width*x, tile_height*y, tile_width, tile_height));
            index++;
        }
    }
    return reg;
}

extern map<int, Texture>            tilemap_register;
extern map<string, Texture>         texture_register;
extern map<string, Font>            font_register;
extern map<string, vector<Texture>> animation_register;

#endif
