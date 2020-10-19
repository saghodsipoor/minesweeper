#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <string>
#include <vector>

struct Direction {int i, j;}; 

struct Cell
{
  struct Index
  {
    int i, j;
  };
  // Index index = {0, 0};
  bool visitted = false, bombed = false, flagged = false;
  unsigned neighbor_bombs = 0;
};


class Game
{
public:
  void reset();
  void finish();
  bool game_is_on();
  const std::string cell_state(const Cell::Index& index) const;
  void toggle_flag(const Cell::Index& index);
  void visit(const Cell::Index& index);
  void print();
  
  struct Size {unsigned w, h;};
  Size size() const;

  Game(Size size = {9, 9});
  ~Game();
private:
  void set_neighbor_bombs_();
  void plant_bombs_();
  inline Cell& cell(const Cell::Index& index)
  { return board_[index.i * size_.w + index.j]; }
  Size size_;
  static const unsigned direction_num = 8;
  const Direction dirs_[direction_num] =
    {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1,-1}, {1,0}, {1,1}};
  bool game_is_on_ = true;
  std::vector<Cell> board_;
};


#endif
