#include "Player.h"

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::NewPos(Point starting_pos) {
  coords.x = old_coords.x = starting_pos.x;
  coords.y = old_coords.y = starting_pos.y;
}

void Player::ProcessInput(MovementDir dir, std::vector<std::vector<int>> tileMap, int &level)
{
  int move_dist = move_speed * 1;

  int x_tile = coords.x / tileSize;
  int y_tile = coords.y / tileSize;

  int curTile = tileMap[y_tile][x_tile], 
      upTile = tileMap[y_tile - 1][x_tile], 
      downTile = tileMap[y_tile + 1][x_tile], 
      rightTile = tileMap[y_tile][x_tile + 1], 
      leftTile = tileMap[y_tile][x_tile - 1];

  if (curTile == -1) 
    level = -1;
  else 
  switch(dir)
  {
    case MovementDir::UP:
      if (upTile < 1) {
        old_coords.y = coords.y;
        coords.y -= move_dist;
      }
      break;
    case MovementDir::DOWN:
      if (downTile < 1) {
        old_coords.y = coords.y;
        coords.y += move_dist;
      }
      break;
    case MovementDir::LEFT:
      if (leftTile < 1) {
        old_coords.x = coords.x;
        coords.x -= move_dist;
      }
      break;
    case MovementDir::RIGHT:
      if (rightTile < 1) {
        old_coords.x = coords.x;
        coords.x += move_dist;
      }
      break;
    case MovementDir::OPEN:
      if (upTile > 1) 
        level = upTile;
    default:
      break;
  }
}

void Player::Draw(Image &screen, Image &cur_level)
{
  for (int i = 0; i < screen.Width(); ++i) 
    for (int j = 0; j < screen.Height(); ++j) 
      screen.PutPixel(i, j, cur_level.GetPixel(i,j));

  if (Moved()) 
  {
    for (int y = old_coords.y; y <= old_coords.y + tileSize; ++y)
      for (int x = old_coords.x; x <= old_coords.x + tileSize; ++x)
        screen.PutPixel(x, y, cur_level.GetPixel(x,y));
    old_coords = coords;
  }

  for (int y = coords.y; y < coords.y + tileSize; ++y)
    for (int x = coords.x; x < coords.x + tileSize; ++x)
    {
      screen.DrawPixel(x, y, Sprite.GetPixel(x - coords.x, y - coords.y));
    }
}
