#include <iostream>
#include "Game.hpp"

int main()
{
  Game g1({9, 9});

  g1.print();
  int i, j;
  while (true)
  {
    char command;
    std::cin >> command;
    std::cin >> i >> j;
    switch (command)
    {
    case 'f':
      g1.toggle_flag({i, j});
      break;
    case 'v':
      g1.visit({i, j});
      break;
    default:
      break;
    }
    g1.print();
  }

  return 0;
}