#include "World.hpp"

using namespace std;

World::World(ifstream *file) {
    /**
     * Read a world from the file and populate the collisions and
     * background vectors.
     */
    
    const unsigned short int width_offset = 0x03;
    const unsigned short int height_offset = 0x04;
    const unsigned short int lvl_offset = 0x05;

    unsigned short int w, h;

    vector<char> contents;
    unsigned char ch;
    unsigned int index = 0;
    while (*file >> noskipws >> ch) {
        // First three characters are "LVL"
        // Next two characters are width and height
        if (index == width_offset) w = ch;
        if (index == height_offset) h = ch;

        index++;
        if (index == lvl_offset) break;
    }

    cout << w * h << endl;

    char *collisions_data = new char[w * h];
    char *background_data = new char[w * h];

    file->get(collisions_data, w * h+1);
    file->get(background_data, w * h);

    for (unsigned int y = 0; y < h; y++) {
        for (unsigned int x = 0; x < w; x++) {
            if (collisions_data[y*w + x] != 0)
                collisions.push_back(Block(x*50, y*50, &tilemap_register[collisions_data[y*w + x]]));
            if (background_data[y*w + x] != 0)
                background.push_back(Block(x*50, y*50, &tilemap_register[background_data[y*w + x]]));
        }
    }

    delete collisions_data;
    delete background_data;
}