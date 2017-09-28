#include "StatusBar.hpp"

StatusBar::StatusBar(Player *player, World *world) {
    this->player = player;
    this->world = world;
}

/**
 * Shows the following information:
 *  Player health
 *  Stats
 */
void StatusBar::draw(RenderWindow *window) {
    RectangleShape bg;
    bg.setSize(Vector2f(window->getSize().x, STATBAR_HEIGHT));
    bg.setPosition(Vector2f(0, window->getSize().y - STATBAR_HEIGHT));
    bg.setFillColor(Color::Black);
    window->draw(bg);

    
}