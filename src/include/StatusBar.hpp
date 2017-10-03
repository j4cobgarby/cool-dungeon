#ifndef STATUSBAR_HPP
#define STATUSBAR_HPP

#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "World.hpp"
#include "asset_registers.hpp"

#define STATBAR_HEIGHT  250
#define STAT_REL_OFFSET 240
#define STAT_OFFSET     45

/** A HUD for the player to keep an eye on stats, health, etc.
 * @param player  A pointer to the player to track the stats, health, etc.
 * @param world   A pointer to the world to track world info
 */
class StatusBar {
private:
  Player *_player;
  World *_world;

  vector<array<string, 2>> _stat_dat {
    {"HEA", "hea"},
    {"ATK", "atk"},
    {"DEF", "def"},
    {"DEX", "dex"},
    {"SCO", "sco"}
  };
public:
  StatusBar(Player *player, World *world);

  void draw(RenderWindow *window);
};

#endif
