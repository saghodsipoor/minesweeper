#include <algorithm>
#include <chrono>
#include <list>
#include <random>
#include <vector>

#include "Game.hpp"

void Game::toggle_flag(const Cell::Index& index)
{
  cells_[index.i][index.j].flagged ^= true;
}

void Game::print() const
{
  for (const auto& row : cells_)
  {
    for (const auto& cell : row)
    {
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

void Game::visit_(Cell *first)
{
  std::list<Cell *> to_visit({first});
  while(to_visit.empty() == false)
  {
    auto& cell = to_visit.front();
    if (cell->bombed)
      break;
    
    if (cell->neighbor_bombs != 0)
    {
      cell->visitted = true;
      to_visit.pop_front();
      continue;
    }
    
    // checking neighbors' condition
    for (const auto& dir : dirs_)
    {
      int i = cell->index.i + dir.i, j = cell->index.j + dir.j;
      // invalid index
      if (i < 0 || i >= size_.w || j < 0 || j >= size_.h)
        continue;
      
      if (cells_[i][j].flagged)
        continue;
      // blank cell
      if (cells_[i][j].visitted == false)
        to_visit.push_back(&cells_[i][j]);
    }
    cell->visitted = true;
    to_visit.pop_front();
  } 
}

void Game::set_neighbor_bombs_()
{
  for (auto i = 0; i < size_.w; ++i)
    for (auto j = 0; j < size_.h; ++j)
    {
      auto& cell = cells_[i][j];
      unsigned num = 0;
      for (auto dir = dirs_; dir < dirs_ + 8; ++dir)
      {
        if (i + dir->i < 0 || i + dir->i >= size_.w ||
          j + dir->j < 0 || j + dir->j >=size_.h)
          { continue; }
        else if(cells_[i+dir->i][j+dir->j].bombed)
          ++num;
      }
      cell.neighbor_bombs = num;
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
    cells_[indices[i].i][indices[i].j].bombed = true;
}

Game::Game(Size size): size_(size), cells_(size.w)
{
  int i = 0;;
  for (auto& row : cells_)
  {
    row.resize(size.h);
    int j = 0;
    for (auto& cell : row)
      cell.index.i = i, cell.index.j = j++;
    ++i;
  }
  plant_bombs_();
  set_neighbor_bombs_();
}

Game::~Game()
{
}
