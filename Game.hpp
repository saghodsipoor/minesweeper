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
  friend Index operator+(const Index& lhs, const Index& rhs)
  { return {lhs.i + rhs.i,  lhs.j + rhs.j}; }

  struct Cell
  {
    bool visitted = false, bombed = false, flagged = false;
    unsigned neighbor_bombs = 0;
  };

  struct Size {int w, h;};
  
  enum GameState {On, Won, Lost};

  void reset();
  bool is_on() { return state_ == On; }
  inline bool index_is_valid(const Index& index);
  // gets a callable having two arguments int i and int j and calls it 
  template <typename Callable> void for_each_index(const Callable& cb);
  const std::string cell_state(const Index& index) const;
  void toggle_flag(const Index& index);
  void visit(const Index& index);
  void print();
  
  Size size() const;
  GameState state() const { return state_; }
  
  Game(Size size = {9, 9});
  ~Game();

private:
  void set_cell_values();
  void plant_bombs();
  void set_game_state();
  Cell& cell(const Index& index)
  { return board_[index.i * size_.w + index.j]; }
  
  Size size_;
  int bombs_num_ = 0;
  const std::vector<Index> dirs_
    {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1,-1}, {1,0}, {1,1}};
  GameState state_ = On;
  std::vector<Cell> board_;
};

template <typename Callable>
void Game::for_each_index(const Callable& callback)
{
  for (auto i = 0; i < size_.w; ++i)
    for (auto j = 0; j < size_.h; ++j)
      callback(i,j);
}

#endif
