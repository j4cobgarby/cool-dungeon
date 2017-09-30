#include "StatusBar.hpp"

StatusBar::StatusBar(Player *player, World *world) {
    this->_player = player;
    this->_world = world;
}

Text make_stat_text(const Vector2f position, const string stat, const int value) {
    Text ret(stat + '\n' + to_string(value), font_register["main_font"]);
    ret.setCharacterSize(85);
    ret.setFillColor(Color::White);
    ret.setPosition(position);
    return ret;
}

/**
 * Shows the following information:
 *  Player health
 *  Stats
 */
void StatusBar::draw(RenderWindow *window) {
    Vector2f bar_topleft(0, window->getSize().y - STATBAR_HEIGHT);

    /**
     * Draw the background
     */
    RectangleShape bg;
    bg.setSize(Vector2f(window->getSize().x, STATBAR_HEIGHT));
    bg.setPosition(bar_topleft);
    bg.setFillColor(Color(0x181425ff));
    window->draw(bg);

    /**
     * Show the stats
     */
    int _stat_ind = 0;
    for (array<string, 2> stat : _stat_dat) {
        window->draw(make_stat_text(
            Vector2f(
                bar_topleft.x + STAT_OFFSET + STAT_REL_OFFSET*_stat_ind, 
                bar_topleft.y + 10
            ), 
            stat.at(0), _player->stats[stat.at(1)]
        ));
        ++_stat_ind;
    }
}