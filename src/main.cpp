#include <SFML/Graphics.hpp>
#include <cmath>
#include <limits>
#include <algorithm>
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

#define SPEED 30

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

    Entity player( // each texture pixel is 30/16 = 1.875 game pixels
        100, 100,                   // Position
        9*1.875, 15*1.875,          // Box size
        30, 30,                     // Rect size
        4*1.875, 1*1.875,           // Texture offset
        0, 0,                       // Initial velocity
        &texture_register["player"] // Memory address of texture
    );

    World world(&level_file);

    bool u=true,d=true,l=true,r=true;
    Clock deltaClock;

    while (window.isOpen()) {
        Time delta = deltaClock.restart();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    
        if (Keyboard::isKeyPressed(Keyboard::A) && l) {
            player.box.vx -= delta.asSeconds() * SPEED;
        } if (Keyboard::isKeyPressed(Keyboard::D) && r) {
            player.box.vx += delta.asSeconds() * SPEED;
        } if (Keyboard::isKeyPressed(Keyboard::W) && u) {
            player.box.vy -= delta.asSeconds() * SPEED;
        } if (Keyboard::isKeyPressed(Keyboard::S) && d) {
            player.box.vy += delta.asSeconds() * SPEED;
        }

        player.box.vx *= 0.9;
        player.box.vy *= 0.9;

        window.clear(Color(0x181425ff));
        
        for (Block block : world.background) {
            window.draw(block.rect);
        }

        for (Block block : world.collisions) {
            window.draw(block.rect);
        }

        window.draw(player.rect);
        RectangleShape player_box;
        player_box.setPosition(Vector2f(player.box.x, player.box.y));
        player_box.setFillColor(Color::Transparent);
        player_box.setOutlineColor(Color::Green);
        player_box.setOutlineThickness(0.5);
        player_box.setSize(Vector2f(player.box.w, player.box.h));
        window.draw(player_box);

        window.display();

        Box broadphasebox = GetSweptBroadphaseBox(player.box);

        float normalx, normaly;
        float collisiontime = 1;

        for (Block block : world.collisions) {
            Box b = block.box;
            float bnormalx, bnormaly, bcollisiontime;
            bcollisiontime = SweptAABB(player.box, b, &bnormalx, &bnormaly);
            if (bcollisiontime != 1 && bcollisiontime < 1 && bcollisiontime < collisiontime && AABBCheck(broadphasebox, b)) {
                collisiontime = bcollisiontime;
                normalx = bnormalx;
                normaly = bnormaly;
            }
        }

        player.box.x += player.box.vx * collisiontime;
        player.box.y += player.box.vy * collisiontime;
        float remainingtime = 1.0f - collisiontime;

        if (remainingtime != 0) {
           float dotprod = (player.box.vx * normaly + player.box.vy * normalx) * remainingtime;
           player.box.vx = dotprod * normaly;
           player.box.vy = dotprod * normalx;
           player.box.vx *= 1.12;
           player.box.vy *= 1.12;

            if (normaly == -1) {
                d = false;
                u = true;
            } else if (normaly == 1) {
                d = true;
                u = false;
            }
            if (normalx == -1) {
                l = true;
                r = false;
            } else if (normalx == 1) {
                l = false;
                r = true;
            }
        } else {
            u = true;
            d = true;
            l = true;
            r = true;
        }

        player.rect.setPosition(Vector2f(player.box.x - player.texture_offset_x, player.box.y - player.texture_offset_y));
        player_view.setCenter(Vector2f(
            lerp(player_view.getCenter().x, player.rect.getPosition().x + 45/2, 0.03), 
            lerp(player_view.getCenter().y, player.rect.getPosition().y + 45/2, 0.03)
        ));
        window.setView(player_view);
    }  

    return 0;
}

float SweptAABB(Box b1, Box b2, float *normalx, float *normaly) {
    float xInvEntry, yInvEntry;
    float xInvExit, yInvExit;

    if (b1.vx > 0.0f) {
        xInvEntry = b2.x - (b1.x + b1.w);
        xInvExit = (b2.x + b2.w) - b1.x;
    }
    else {
        xInvEntry = (b2.x + b2.w) - b1.x;
        xInvExit = b2.x - (b1.x + b1.w);
    }
    if (b1.vy > 0.0f) {
        yInvEntry = b2.y - (b1.y + b1.h);
        yInvExit = (b2.y + b2.h) - b1.y;
    }
    else {
        yInvEntry = (b2.y + b2.h) - b1.y;
        yInvExit = b2.y - (b1.y + b1.h);
    }

    float xEntry, yEntry;
    float xExit, yExit;
    if (b1.vx == 0.0f) {
        xEntry = -std::numeric_limits<float>::infinity();
        xExit = std::numeric_limits<float>::infinity();
    }
    else {
        xEntry = xInvEntry / b1.vx;
        xExit = xInvExit / b1.vx;
    }
    if (b1.vy == 0.0f) {
        yEntry = -std::numeric_limits<float>::infinity();
        yExit = std::numeric_limits<float>::infinity();
    }
    else {
        yEntry = yInvEntry / b1.vy;
        yExit = yInvExit / b1.vy;
    }

    if (yEntry > 1.0f) yEntry = -numeric_limits<float>::max();
    if (xEntry > 1.0f) xEntry = -numeric_limits<float>::max();

    float entryTime = max(xEntry, yEntry);
    float exitTime = min(xExit, yExit);

    if (entryTime > exitTime || (xEntry < 0.0f && yEntry < 0.0f) || xEntry > 1.0f || yEntry > 1.0f) {
        *normalx = 0.0f;
        *normaly = 0.0f;
        return 1.0f;
    }

    else {
        if (xEntry > yEntry) {
            if (xInvEntry < 0.0f) {
                *normalx = 1.0f;
                *normaly = 0.0f;
            }
            else {
                *normalx = -1.0f;
                *normaly = 0.0f;
            }
        }
        else
            {
            if (yInvEntry < 0.0f) {
                *normalx = 0.0f;
                *normaly = 1.0f;
            }
            else {
                *normalx = 0.0f;
                *normaly = -1.0f;
            }
        }
        return entryTime - 0.1;
    }
}

Box GetSweptBroadphaseBox(Box b) {
    Box broadphasebox;
    broadphasebox.x = b.vx > 0 ? b.x : b.x + b.vx;
    broadphasebox.y = b.vy > 0 ? b.y : b.y + b.vy;
    broadphasebox.w = b.vx > 0 ? b.vx + b.w : b.w - b.vx;
    broadphasebox.h = b.vy > 0 ? b.vy + b.h : b.h - b.vy;
    return broadphasebox;
}

bool AABBCheck(Box b1, Box b2) {
    return !(b1.x + b1.w < b2.x || b1.x > b2.x + b2.w || b1.y + b1.h < b2.y || b1.y > b2.y + b2.h);
}