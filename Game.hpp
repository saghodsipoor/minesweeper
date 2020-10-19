#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <string>
#include <vector>

class Game
{
public:
  struct Index
  {
    int i, j;
    Index(int i, int j) : i(i), j(j) {}
  };
  struct Cell
  {
    bool visitted = false, bombed = false, flagged = false;
    unsigned neighbor_bombs = 0;
  };

  friend Index operator+(const Index& lhs, const Index& rhs)
  { return {lhs.i + rhs.i,  lhs.j + rhs.j}; }

  void reset();
  void finish();
  bool game_is_on();
  inline bool index_is_valid(const Index& index);
  const std::string cell_state(const Index& index) const;
  void toggle_flag(const Index& index);
  void visit(const Index& index);
  void print();
  
  struct Size {unsigned w, h;};
  Size size() const;

  Game(Size size = {9, 9});
  ~Game();

private:
  void set_neighbor_bombs_();
  void plant_bombs_();
  inline Cell& cell(const Index& index)
  { return board_[index.i * size_.w + index.j]; }
  Size size_;
  static const unsigned direction_num = 8;
  const std::vector<Index> dirs_
    {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1,-1}, {1,0}, {1,1}};
  bool game_is_on_ = true;
  std::vector<Cell> board_;
};


#endif
