#include <cstdlib>

#include <iostream>
#include <string>

#include "Game.hpp"
#include "Minesweeper.hpp"

int main(int argv, char *argc[])
{
  int i = 10, j = 10;
  switch (argv)
  {
  case 1:
    break;
  
  case 2:
    i = atoi(argc[1]);
    break;
  
  case 3:
    i = atoi(argc[1]), j = atoi(argc[2]);
    break;
  
  default:
    break;
  }

  Game g({i, j});
  Minesweeper ms(g);
  ms.start();
  
  return 0;
}
