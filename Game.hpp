#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>

struct Direction {int i, j;}; 


struct Cell
{
  struct Index
  {
    int i, j;
  };
  Index index = {0, 0};
  bool visitted = false, bombed = false, flagged = false;
  unsigned neighbor_bombs = 0;
};


class Game
{
public:
  // dbg
  void visit(Cell::Index ind) { visit_(&cells_[ind.i][ind.j] ); }
  ///
  void print() const;
  struct Size {unsigned w, h;};
  Game(Size size = {9, 9});
  ~Game();
private:
  void visit_(Cell *first);
  void set_neighbor_bomb_();
  void plant_bombs_();
  Size size_;
  static const unsigned direction_num = 8;
  const Direction dirs_[direction_num] =
    {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1,-1}, {1,0}, {1,1}};
  std::vector<std::vector<Cell> > cells_;
};


#endif
