#include <algorithm>
#include <chrono>
#include <list>
#include <random>
#include <vector>

#include "Game.hpp"

void Game::reset()
{
  board_.clear();
  board_.resize(size_.w * size_.h);
  plant_bombs_();
  set_neighbor_bombs_();
  game_is_on_ = true;
}

void Game::finish()
{
  game_is_on_ = false;
}

bool Game::game_is_on() 
{ 
  return game_is_on_;
}

inline bool Game::index_is_valid(const Index& index)
{
  return index.i >= 0 && index.i < size_.w && index.j >= 0 && index.j < size_.h;
}

const std::string Game::cell_state(const Index& index) const
{
  const auto& cell = board_[index.i * size_.w + index.j];
  
  if (cell.flagged)
    return "flagged";

  if (!cell.visitted)
    return "unclicked";

  // if visitted
  if (cell.bombed)
    return "bomb-clicked";

  if (cell.neighbor_bombs == 0)
    return "blank";

  return std::to_string(cell.neighbor_bombs);
}

void Game::toggle_flag(const Index& index)
{
  auto& cell = this->cell(index);
  if (!cell.visitted)
    cell.flagged = !cell.flagged;
}

void Game::print()
{
  for (int i = 0; i < size_.w; ++i)
  {
    for (int j = 0; j < size_.h; ++j)
    {
      const auto& cell = this->cell({i,j});
      if (cell.flagged)
        std::cout << "\e[38;5;46m" << " f " << "\e[0m";
      else
      if (cell.bombed)
        std::cout << "\e[38;5;196m" << " b " << "\e[0m";
      else
      if (cell.visitted)
        std::cout << "\e[38;5;26m " << cell.neighbor_bombs << " \e[0m";
      else
        std::cout << " " << cell.neighbor_bombs << " ";
    }
    std::cout << std::endl;
  }
}

void Game::visit(const Index& index)
{
  // auto first = &cells_[index.i][index.j];
  auto first = index;
  if (cell(first).flagged)
    return;
  
  // if cell isn't flagged
  std::list<Index> to_visit {first};
  while(!to_visit.empty())
  {
    auto curr_index = to_visit.front();
    auto& curr_cell = cell(curr_index);
    curr_cell.visitted = true;

    if (curr_cell.bombed)
    {
      game_is_on_ = false;
      break;
    }
    if (curr_cell.neighbor_bombs != 0)
    {
      to_visit.pop_front();
      continue;
    }
    
    // checking neighbors' condition
    for (const auto& dir : dirs_)
    {
      auto neighbor_index = curr_index + dir;
      // int i = curr_index.i + dir.i, j = curr_index.j + dir.j;
      // invalid index
      if (!index_is_valid(neighbor_index))
        continue;
      
      if (cell(neighbor_index).flagged)
        continue;
      // blank cell
      if (!cell(neighbor_index).visitted)
        to_visit.push_back(neighbor_index);
    }
    to_visit.pop_front();
  } 
}

Game::Size Game::size() const 
{
  return size_;
}

void Game::set_neighbor_bombs_()
{
  for (auto i = 0; i < size_.w; ++i)
    for (auto j = 0; j < size_.h; ++j)
    {
      unsigned num = 0;
      for (const auto& dir : dirs_)
      {
        auto index = Index(i,j) + dir;
        if (!index_is_valid(index))
          { continue; }
        else
        if(cell(index).bombed)
          ++num;
      }
      cell({i,j}).neighbor_bombs = num;
    }
}

void Game::plant_bombs_()
{
  unsigned bombs_num = size_.h * size_.w / 10;
  
  // make list of indices
  std::vector<Index> indices;

  for (int i = 0; i < size_.w; ++i)
    for (int j = 0; j < size_.h; ++j)
      indices.push_back({i,j}); 

  // shuffle indices
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(indices.begin(), indices.end(), std::default_random_engine(seed));

  // plant bombs for list of indices
  for (auto i = 0; i < bombs_num; ++i)
    cell(indices[i]).bombed = true;
}

Game::Game(Size size) : size_(size), board_(size.w * size.h)
{
  plant_bombs_();
  set_neighbor_bombs_();
}

Game::~Game()
{
}
