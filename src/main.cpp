#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdarg>

#include "include/World.hpp"
#include "include/asset_registers.hpp"
#include "include/Player.hpp"
#include "include/StatusBar.hpp"
#include "include/Baddie.hpp"
#include "include/BaddieTypes.hpp"

using namespace std;
using namespace sf;

/** Populate registers */
map<int, Texture> tilemap_register = init_tilemap_register("assets/images/tilemap.png", 16, 16, 16, 16);

map<string, vector<Texture>> animation_register {
    {"explode",             makeAnimation("assets/images/explode.png",          16, 16, 16)},
    {"hit",                 makeAnimation("assets/images/hit-effect.png",       11, 16, 16)},
    {"swipe",               makeAnimation("assets/images/swipe.png",            12, 16, 16)},
    {"jab",                 makeAnimation("assets/images/jab.png",              14, 16, 16)},

    {"player_idle_rt",      makeAnimation("assets/images/player_idle_rt.png",   1, 16, 16)},
    {"player_idle_lt",      makeAnimation("assets/images/player_idle_lt.png",   1, 16, 16)},
    {"player_walk_rt",      makeAnimation("assets/images/player_walk_rt.png",   2, 16, 16)},
    {"player_walk_lt",      makeAnimation("assets/images/player_walk_lt.png",   2, 16, 16)},
    {"player_walk_uprt",    makeAnimation("assets/images/player_walk_uprt.png", 2, 16, 16)},
    {"player_walk_uplt",    makeAnimation("assets/images/player_walk_uplt.png", 2, 16, 16)},

    {"ghost",               makeAnimation("assets/images/ghost.png",            4, 16, 16)},
};

map<string, Texture> texture_register {
    {"player", makeTexture("assets/images/player.png")},
    {"sword1", makeTexture("assets/images/sword.png")},
    {"cursor", makeTexture("assets/images/cursor.png")},
};

map<string, Font> font_register {
    {"main_font", makeFont("assets/fonts/I-pixel-u.ttf")},
};

int main() {
    RenderWindow window(sf::VideoMode(1500, 1400), "A cool dungeon game", Style::None | Style::Titlebar | Style::Close);
    Sprite cursor(texture_register["cursor"]);
    View player_view(Vector2f(200, 200), Vector2f(1500, 1400));
    ifstream level_file("levels/default/1.level", ios::in | ios::binary);
    Entity test_ent(100, 100, 30, 30, 30, 30, 0, 0, 0, 0, NULL);
    World world(&level_file);
    Player player(world.spawn_position, Weapon("A cool sword", &texture_register["sword1"], 3, 100));
    StatusBar statbar(&player, &world);
    window.setMouseCursorVisible(false);
    vector<Baddie> baddies;
    player.set_baddies(&baddies);

    Clock global_clock;
    Clock deltaClock;

    srand(time(NULL));

    cursor.setScale(8, 8);
    player_view.zoom(0.2);
    window.setView(player_view);
    baddies.push_back(Ghost(Vector2f(500, 60), &player));
    baddies.push_back(Ghost(Vector2f(300, 60), &player));

    if (!level_file.is_open()) return -1;

    while (window.isOpen()) {
        Time delta = deltaClock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    player.click(&delta, &world, &window);
                }
            }
        }

        cursor.setPosition((Vector2f)Mouse::getPosition(window));

        player.update(&delta, &global_clock, &world, &window, &cursor.getPosition());
        for (size_t i = 0; i < baddies.size(); i++) {
            if (baddies[i].health <= 0) {
                baddies.erase(baddies.begin() + i);
                i--;
                continue;
            }
            baddies[i].update(&delta, &global_clock, &world, &window, &baddies);
        }

        window.clear(Color(0x181425ff));

        for (Block block : world.background) window.draw(block.rect);
        for (Block block : world.collisions) window.draw(block.rect);
        window.draw(player.rect);
        window.draw(player.weapon.rect);
        for (Baddie baddie : baddies) {
            window.draw(baddie.rect);
            baddie.drawhealthbar(&window);
        }

        window.setView(window.getDefaultView());
        statbar.draw(&window);
        window.draw(cursor);
        window.setView(player_view);

        window.display();

        player_view.setCenter(Vector2f(
            lerp(player_view.getCenter().x, player.rect.getPosition().x + 45/2, 1),
            lerp(player_view.getCenter().y, player.rect.getPosition().y + 45/2, 1)
        ));
        window.setView(player_view);
    }

    return 0;
}
