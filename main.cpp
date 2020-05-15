#include <iostream>

#include "Game.hpp"
#include "graphics.hpp"

int main()
{
  Game g1({10, 10});
  Minesweeper mi(g1);

  return 0;
}
