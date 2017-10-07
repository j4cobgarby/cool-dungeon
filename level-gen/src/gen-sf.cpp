#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

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

map<int, Texture> init_tilemap_register(const string filename,
    const unsigned int tiles_x, const unsigned int tiles_y, // Amount of tiles on each axis
    const unsigned int tile_width, const unsigned int tile_height) { // Width in px of each tile texture
    map<int, Texture> reg;
    unsigned short int index = 1; // 0 = air, start textures at 1
    for (unsigned int y = 0; y < tiles_y; y++) {
        for (unsigned int x = 0; x < tiles_x; x++) {
            cout << "Texture register " << index << " => " << filename << ":" << tile_width*x << ":" << tile_height*y << '\n';
            reg[index] = makeTexture(filename, IntRect(tile_width*x, tile_height*y, tile_width, tile_height));
            index++;
        }
    }
    return reg;
}

bool mouse_over(Vector2f position, RenderWindow *window) {
    Vector2f mousepos = window->mapPixelToCoords(Mouse::getPosition(*window));
    if (mousepos.x > position.x && mousepos.x < position.x + 20 && mousepos.y > position.y && mousepos.y < position.y + 20) return true;
    return false;
}

string save_path;

int main() {
    cout << "Welcome to the graphical level designer for my dungeon game!\n\n";
    cout << "First of all, where would you like to save your level?\nSAVE PATH: ";
    cin >> save_path;

    ofstream outfile(save_path, ios::out | ios::binary);
    if (!outfile.is_open()) return -1;

    unsigned int w, h;

    cout << "Now type the width, followed by <ENTER>, followed by the height.\nWIDTH: \n";
    cin >> w;
    cout << "HEIGHT: \n";
    cin >> h;

    outfile << "LVL";
    outfile.put(w);
    outfile.put(h);

    string tilemap_path = "../assets/images/tilemap.png";

    unsigned int tilesx = 16, tilesy  = 16, tilewidth = 16, tileheight = 16;

    map<int, Texture> tilemap = init_tilemap_register(tilemap_path, tilesx, tilesy, tilewidth, tileheight);
    int c_id = 1;

    RenderWindow window(VideoMode(1500, 1500), "Level editor");
    View view(Vector2f(0, 0), Vector2f(1500, 1500));
    view.zoom(0.3);
    window.setView(view);

    vector<int> blocks;
    for (int i = 0; i < w * h; i++) {
        blocks.push_back(0);
    }

    cout << "Finished initialization, opening window..." << endl;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Q && c_id - 1 >= 0) c_id--;
                if (event.key.code == Keyboard::E && c_id + 1 < tilemap.size()) c_id++;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::W)) view.setCenter(Vector2f(view.getCenter().x, view.getCenter().y - 1));
        if (Keyboard::isKeyPressed(Keyboard::S)) view.setCenter(Vector2f(view.getCenter().x, view.getCenter().y + 1));
        if (Keyboard::isKeyPressed(Keyboard::A)) view.setCenter(Vector2f(view.getCenter().x - 1, view.getCenter().y));
        if (Keyboard::isKeyPressed(Keyboard::D)) view.setCenter(Vector2f(view.getCenter().x + 1, view.getCenter().y));
        window.setView(view);

        window.clear(Color::Black);

        for (int b_index = 0; b_index < w * h; b_index++) {
            RectangleShape current_block;
            current_block.setSize(Vector2f(20, 20));
            current_block.setFillColor((blocks[b_index] == 0 ? Color::Transparent : Color::White));
            current_block.setPosition(Vector2f((b_index % w) * 20, (floor(b_index / w)) * 20));

            if (mouse_over(Vector2f((b_index % w) * 20, (floor(b_index / w)) * 20), &window)) {
                current_block.setOutlineColor(Color::Green);
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    current_block.setFillColor(Color::Yellow);
                    blocks.at(b_index) = c_id;
                }
				if (Mouse::isButtonPressed(Mouse::Right)) {	// Right click to erase block
					current_block.setFillColor(Color::Yellow);
					blocks.at(b_index) = 0;
				}
            } else {
                current_block.setOutlineColor(Color(0x686868ff));
            }
            current_block.setOutlineThickness(0.5);
            current_block.setTexture(&tilemap[blocks[b_index]]);
            window.draw(current_block);
        }

        window.setView(window.getDefaultView());
        RectangleShape preview;
        preview.setTexture(&tilemap[c_id]);
        preview.setSize(Vector2f(200, 200));
        preview.setPosition(Vector2f(0, 0));
        window.draw(preview);
        window.setView(view);

        window.display();
    }


    for (int i = 0; i < blocks.size(); i++) {
        outfile.put(blocks.at(i));
    }
    for (int i = 0; i < blocks.size(); i++) {
        outfile.put(0);
    }

	// Structure of data after the tile data:
	//
	// spawn x
	// spawn y
	// exit position x
	// exit position y

    cout << "spawnx<ENTER>spawny<ENTER>exitx<ENTER>exity<ENTER>\n";

    unsigned int sx, sy, ex, ey;
    cin >> sx;
    cin >> sy;
    cin >> ex;
    cin >> ey;

	outfile.put(sx);
	outfile.put(sy);

	outfile.put(ex);
	outfile.put(ey);

    cout << "Success!" << endl;
    system("pause");
}

/*
for (int i = 0; i < 1800; i++) {
        outfile.put(0);
}
*/
