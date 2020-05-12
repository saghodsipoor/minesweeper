#include <iostream>
#include "Game.hpp"

int main()
{
  Game g1({14, 12});

  g1.print();
  int i, j;
  while (true)
  {
    std::cin >> i >> j;
    g1.visit({i, j});
    g1.print();
  }

  return 0;
}