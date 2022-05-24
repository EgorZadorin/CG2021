#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include <vector>

struct Point
{
  int x;
  int y;
};

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
  OPEN
};

struct Player
{
  explicit Player(Point pos, const std::string &sprite_path) :
    coords(pos), old_coords(coords), Sprite(sprite_path) {};
  bool Moved() const;
  void NewPos(Point starting_pos);
  void ProcessInput(MovementDir dir, std::vector<std::vector<int>> tileMap, int &level);
  void Draw(Image &screen, Image &cur_level);
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};

private:
  int move_speed = 16;
  Image Sprite;
};

#endif //MAIN_PLAYER_H
