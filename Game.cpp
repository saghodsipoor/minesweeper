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

const std::string Game::cell_state(const Cell::Index& index) const
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

void Game::toggle_flag(const Cell::Index& index)
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

void Game::visit(const Cell::Index& index)
{
  // auto first = &cells_[index.i][index.j];
  auto first = index;
  if (cell(first).flagged)
    return;
  
  // if cell isn't flagged
  std::list<Cell::Index> to_visit {first};
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
      int i = curr_index.i + dir.i, j = curr_index.j + dir.j;
      // invalid index
      if (i < 0 || i >= size_.w || j < 0 || j >= size_.h)
        continue;
      
      if (cell({i,j}).flagged)
        continue;
      // blank cell
      if (!cell({i,j}).visitted)
        to_visit.push_back({i,j});
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
      // auto& cell = cells_[i][j];
      unsigned num = 0;
      for (auto dir = dirs_; dir < dirs_ + 8; ++dir)
      {
        if (i + dir->i < 0 || i + dir->i >= size_.w ||
          j + dir->j < 0 || j + dir->j >=size_.h)
          { continue; }
        else
        if(cell({i+dir->i, j+dir->j}).bombed)
          ++num;
      }
      cell({i,j}).neighbor_bombs = num;
    }
}

void Game::plant_bombs_()
{
  unsigned bombs_num = size_.h * size_.w / 10;
  
  // make list of indices
  std::vector<Cell::Index> indices;

  for (int i = 0; i < size_.w; ++i)
    for (int j = 0; j < size_.h; ++j)
    {
      const Cell::Index& index = {i, j};
      indices.push_back(index); 
    }

  // shuffle indices
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(indices.begin(), indices.end(), std::default_random_engine(seed));

  // plant bombs for list of indices
  for (auto i = 0; i < bombs_num; ++i)
    cell(indices[i]).bombed = true;
}

Game::Game(Size size): size_(size), board_(size.w * size.h)
{
  plant_bombs_();
  set_neighbor_bombs_();
}

Game::~Game()
{
}
