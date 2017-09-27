#ifndef STATUSBAR_HPP
#define STATUSBAR_HPP

#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "World.hpp"
#include "tex_register.hpp"

/** A HUD for the player to keep an eye on stats, health, etc.
 * @param player  A pointer to the player to track the stats, health, etc.
 * @param world   A pointer to the world to track world info
 */
class StatusBar {
private:
  Player *player;
  World *world;
public:
  StatusBar(Player *player, World *world);

  void draw(RenderWindow *window);
  void update();
};

#endif
