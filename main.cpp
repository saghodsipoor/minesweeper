#include <iostream>

#include "Game.hpp"
#include "graphics.hpp"

int main()
{
  Game g1({20, 15});
  Minesweeper mi(g1);

  // g1.print();
  // int i, j;
  // while (true)
  // {
  //   char command;
  //   std::cin >> command;
  //   std::cin >> i >> j;
  //   switch (command)
  //   {
  //   case 'f':
  //     g1.toggle_flag({i, j});
  //     break;
  //   case 'v':
  //     g1.visit({i, j});
  //     break;
  //   default:
  //     break;
  //   }
  //   g1.print();
  // }

  return 0;
}
