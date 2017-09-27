#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdarg>

#include "Entity.hpp"
#include "World.hpp"
#include "Block.hpp"
#include "tex_register.hpp"
#include "Maths.hpp"
#include "AABB.hpp"
#include "Player.hpp"

using namespace std;
using namespace sf;

float SweptAABB(Box b1, Box b2, float *normalx, float *normaly);
Box GetSweptBroadphaseBox(Box b);
bool AABBCheck(Box b1, Box b2);

Texture tex_wall_normal;
Texture tex_floor_normal;

Texture makeTexture(string filename) {
    Texture tex;
    tex.loadFromFile(filename);
    return tex;
}

Texture makeTexture(string filename, IntRect area) {
    Texture tex;
    tex.loadFromFile(filename, area);
    return tex;
}

map<int, Texture> init_tilemap_register(const string filename,
        const unsigned int tiles_x, const unsigned int tiles_y, // Amount of tiles on each axis
        const unsigned int tile_width, const unsigned int tile_height) { // Width in px of each tile texture
    map<int, Texture> reg;
    unsigned short int index = 1;
    for (unsigned int y = 0; y < tiles_y; y++) {
        for (unsigned int x = 0; x < tiles_x; x++) {
            cout << "Texture register " << index << " => " << filename << ":" << tile_width*x << ":" << tile_height*y << '\n';
            reg[index] = makeTexture(filename, IntRect(tile_width*x, tile_height*y, tile_width, tile_height));
            index++;
        }
    }
    return reg;
}

map<string, Texture> init_texture_register() {
    map<string, Texture> reg;

    reg["player"] = makeTexture("assets/player.png");
    reg["sword1"] = makeTexture("assets/sword-1.png");

    return reg;
}

/** Populate texture register */
map<int, Texture> tilemap_register = init_tilemap_register("assets/tilemap.png", 5, 5, 16, 16);
map<string, Texture> texture_register = init_texture_register();

int main() {
    RenderWindow window(sf::VideoMode(1200, 900), "Test world");
    View player_view(Vector2f(200, 200), Vector2f(1200, 900));
    player_view.zoom(0.2);
    window.setView(player_view);

    ifstream level_file("levels/default/1.level", ios::in | ios::binary);
    if (!level_file.is_open()) return -1;

    Player player(Vector2f(100, 100), Weapon("Sword...", &texture_register["sword1"], 3, 100));

    World world(&level_file);

    Clock deltaClock;

    while (window.isOpen()) {
        Time delta = deltaClock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.update(&delta, &world, &window);

        window.clear(Color(0x181425ff));

        for (Block block : world.background) window.draw(block.rect);
        for (Block block : world.collisions) window.draw(block.rect);
        window.draw(player.rect);
        window.draw(player.weapon.rect);

        window.display();

        player_view.setCenter(Vector2f(
            lerp(player_view.getCenter().x, player.rect.getPosition().x + 45/2, 1),
            lerp(player_view.getCenter().y, player.rect.getPosition().y + 45/2, 1)
        ));
        window.setView(player_view);
    }

    return 0;
}
